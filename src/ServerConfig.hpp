
#ifndef SERVER_CONFIG_HPP

# define SERVER_CONFIG_HPP

# include <string>
# include <vector>
# include <map>
# include "Location.hpp"


struct ServerConfig {
	std::string							_server_name;
	std::string							_port;
	std::string							_host;
	std::string							_root;
	size_t								_max_body;
	std::map<int, std::string>			_error_pages;
	std::vector<std::string>			_index;
	bool								_auto_index;
	std::vector<Location>				_locations;
	std::vector<std::string>			_allowed_methods;
	std::string							_cgi_path;
	bool								_upload;
	std::string							_upload_dir;


};
#endif