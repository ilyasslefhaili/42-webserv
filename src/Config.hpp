
#ifndef CONFIG_HPP


# define CONFIG_HPP

# include <vector>
# include "ServerConfig.hpp"
# include <string>
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>
# include <stdexcept>


// this class will parse the config file and create a config class for every server

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


};

#endif