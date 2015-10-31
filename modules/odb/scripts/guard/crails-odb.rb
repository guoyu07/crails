require 'guard/crails-base'
require 'json'

module ::Guard
  class CrailsOdb < CrailsPlugin
    def run_all
      run_on_modifications watched_files
    end

    def run_on_modifications(paths)
      files = fetch_odb_files_in paths
      if files.count > 0
        compile_models files
      else
        puts "[crails-odb] Nothing to compile"
      end
    end

  private
    def fetch_odb_files_in paths
      odb_files = []
      paths.each do |path|
        content = File.read path
        odb_files << path if content =~ /^\s*#\s*pragma\s+db/
      end
      odb_files
    end

    def odb_options
      options     = ""
      databases   = JSON.parse File.read 'config/db.json'
      environment = if developer_mode? then 'development' else 'production' end
      databases   = databases[environment]
      db_types    = []
      databases.each do |key, options|
        next if key != 'odb'
        db_type   = options['type'] || 'sqlite'
        db_types += [ db_type ]
      end
      options = "--multi-database dynamic -d common " if db_types.count > 1
      options + "-d " + (db_types.uniq.join " -d ") + " --generate-query --generate-schema"
    end

    def compile_models paths
      cmd = "odb #{odb_options} #{paths.join ' '}"
      puts "+ #{cmd}"
      `#{cmd}`
    end
  end
end