
#ifndef CONFIG_HPP


# define CONFIG_HPP

# include <vector>
# include <string>
# include <cstring>
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>
# include <stdexcept>

# include "Server.hpp"
# include "ServerConfig.hpp"


// this class will parse the config file and create a config class for every server



class Server;

class Config {
    private:
        std::vector<ServerConfig>   _configs;
        std::string                 _file_path;


    public:
        // Config();
        Config(const std::string &file_path);
        ~Config();
        Config(const Config & src);
        Config &operator=(const Config & rhs);

        void    parse();
        void    parse_location(std::string &key, std::istringstream &ss, Location &current_location);
        void    print();
        
        bool    is_port_valid(const std::string &port);

        void generate_servers(std::vector<Server> &servers);

        // there are some parameters that are set if not stated in the config file
        // example host : 127.0.0.1 by default
        void                init_if_not_set();

        
        std::vector<ServerConfig> &get_configs();

        class ConfigFileException : public std::exception
        {
			public:
				virtual const char * what() const throw();
        };


};

#endif