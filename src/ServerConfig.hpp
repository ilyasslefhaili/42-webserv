
#ifndef SERVER_CONFIG_HPP

# define SERVER_CONFIG_HPP

# include <string>
# include <map>
# include <vector>
# include "Location.hpp"



class ServerConfig {
    public:
        std::string                         _server_name;
        std::string                         _port;
        std::string                         _host;
        std::string                         _root;
        std::string                         _return;
        size_t                              _max_body;
		std::map<std::string, std::string>	_error_pages;
        std::string                         _index;
        bool                                _auto_index;
        std::vector<Location>               _locations;

    // public:
    //     ServerConfig();
    //     ~ServerConfig();
    //     ServerConfig(ServerConfig const &src);
    //     ServerConfig &operator=(ServerConfig const &rhs);




};

#endif