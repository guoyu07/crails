#ifndef  DATABASES_HPP
# define DATABASES_HPP

# include <map>
# include <vector>
# include <boost/thread/tss.hpp>
# include "datatree.hpp"
# include "environment.hpp"

# define CRAILS_DATABASE(type,database) \
  Crails::Databases::singleton().get_database<type::Database>(database)

# define CRAILS_DATABASE_FROM_SETTINGS(type,database,settings) \
  Crails::Databases::singleton().get_database<type::Database>(database,settings)

namespace Crails
{
  class Databases
  {
  public:
    typedef std::map<std::string, boost::any> DatabaseSettings;
    typedef std::map<std::string, std::map<std::string, DatabaseSettings> > Settings;

    static const Settings settings;

    class Db
    {
      friend class Databases;
    public:
      Db(const std::string& type) : type(type) {}
      virtual ~Db();

      bool              operator==(const std::string& name) const { return this->name == name; }
      const std::string& get_type() const { return type; }
      const std::string& get_name() const { return name; }
      virtual void      connect(void) = 0;

    protected:
      std::string       name;
      std::string       type;
    };

    typedef std::vector<Db*> Dbs;

    struct Exception : public std::exception
    {
      Exception(const std::string& message) : message(message) {}

      const char* what() const throw() { return (message.c_str()); }
      std::string message;
    };

    Databases()
    {
    }

    ~Databases()
    {
      cleanup_databases();
    }

    static Databases& singleton();

    void cleanup_databases();

    Db* get_database_from_name(const std::string& key);

    template<typename TYPE>
    Db* initialize_database(const std::string& key, const Crails::Databases::DatabaseSettings& settings)
    {
      TYPE* database = new TYPE(settings);
      Db*   database_as_db = database;

      database_as_db->name = key;
      databases.push_back(database);
      return database;
    }

    template<typename TYPE>
    TYPE& get_database(const std::string& key, const Crails::Databases::DatabaseSettings& settings)
    {
      Db* db = get_database_from_name(key);

      if (!db)
        db = initialize_database<TYPE>(key, settings);
      if (db->get_type() != TYPE::ClassType())
        throw Databases::Exception("Expected type '" + TYPE::ClassType() + "', got '" + db->get_type() + '\'');
      db->connect();
      return (*(reinterpret_cast<TYPE*>(db)));
    }

    template<typename TYPE>
    TYPE& get_database(const std::string& key)
    {
      auto environment_settings = settings.at(Crails::environment);

      return get_database<TYPE>(key, environment_settings[key]);
    }

  private:
    Dbs      databases;
  };

  extern boost::thread_specific_ptr<Databases> databases;
}

#endif
