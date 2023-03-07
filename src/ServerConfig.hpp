
#ifndef SERVER_CONFIG_HPP

# define SERVER_CONFIG_HPP

# include <string>
# include <vector>
# include <map>

struct ServerConfig {
        std::string                         _server_name;
        std::string                         _port;
        std::string                         _host;
        std::string                         _root;
        size_t                              _max_body;
		std::map<std::string, std::string>	_error_pages;
        std::vector<std::string>             _index;
        bool                                _auto_index;
        std::vector<Location>               _locations;
};
#endif