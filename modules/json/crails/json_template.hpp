#ifndef  JSON_TEMPLATE_HPP
# define JSON_TEMPLATE_HPP

# include <functional>
# include <sstream>
# include <string>
# include <crails/template.hpp>

namespace Crails
{
  class JsonTemplate : public Template
  {
  public:
    JsonTemplate(const Renderer* renderer, SharedVars& vars) : 
      Template(renderer, vars),
      first_item_in_object(true)
    {
    }

    template<typename T>
    void json(const std::string& key, const T val)
    {
      add_separator();
      add_key(key);
      add_value(val);
    }

    template<typename ITERATOR>
    void json_array(const std::string& key, ITERATOR beg, ITERATOR end)
    {
      add_separator();
      add_key(key);
      stream << '[';
      while (beg != end)
      {
        add_value(*beg);
        ++beg;
      }
      stream << ']';
    }

    template<typename ITERATOR, typename T>
    void json_array(const std::string& key,
                    ITERATOR beg,
                    ITERATOR end,
                    std::function<void (T)> func)
    {
      add_separator();
      add_key(key);
      stream << '[';
      while (beg != end)
      {
        add_object([this, func, beg]() { func(*beg); });
        ++beg;
      }
      stream << ']';
    }

  protected:
    std::stringstream stream;
  private:
    template<typename T>
    void        add_value(const T val)
    {
      stream << val;
    }

    void        add_separator();
    void        add_key(const std::string& key);
    void        add_object(std::function<void()> func);
    std::string javascript_escape(const std::string& val) const;

    bool first_item_in_object;
  };
}

#endif