
#include "Config.hpp"


Config::Config(const std::string &file_path)
    : _file_path(file_path)
{

}

Config::~Config()
{

}

Config::Config(const Config & src)
{
	*this = src;
}

Config &Config::operator=(const Config & rhs)
{
	this->_file_path = rhs._file_path;
	this->_configs = rhs._configs;
	return (*this);
}

void	Config::print()
{
	std::vector<ServerConfig>::iterator it = _configs.begin();
	while (it != _configs.end())
	{
		std::cout << "server_name: " << it->_server_name << std::endl;
		std::cout << "host: " << it->_host << std::endl;
		std::cout << "index: " << it->_index << std::endl;
		std::cout << "root: " << it->_root << std::endl;
		std::cout << "max_body: " << it->_max_body << std::endl;
		std::cout << "return: " << it->_return << std::endl;

		std::map<std::string, std::string>::iterator err = it->_error_pages.begin();
		while (err != it->_error_pages.end())
		{
			std::cout << err->first << " " << err->second << std::endl;
			++err;
		}
		std::vector<Location>::iterator lo = it->_locations.begin();
		while (lo != it->_locations.end())
		{
			std::cout << "location: " << lo->_path << std::endl;
			std::cout << "	" << "_root: " << lo->_root << std::endl;
			std::cout << "	" << "_ret: " << lo->_ret << std::endl;
			// std::cout << "	" << "_path: " << lo->_path << std::endl;
			std::cout << "	" << "_index: " << lo->_index << std::endl;
			std::cout << "	" << "auto_index: " << lo->_autoindex << std::endl;

			int i = 0;
			std::cout << "   allowed methods: " << std::endl;
			while (i < lo->_allowed_methods.size())
			{
				std::cout << "  " << lo->_allowed_methods[i] << std::endl;
				i++;
			}
			// std::cout << "	" << "_cgi_path: " << lo->_cgi_path[0] << std::endl;
			// std::cout << "	" << "_cgi_ext: " << lo->_cgi_ext[0] << std::endl;
			++lo;
		}

		std::cout << "==============" << std::endl;
		++it;
	}
}

// as long a the key/values are seperated with white spaces in config files
// the parse will work
void    Config::parse()
{
   	std::ifstream file(_file_path);
	ServerConfig	current;
	std::string		line;

	current._port = "-1";

	bool parsing_location = false; // set to true when we are parsing the location block

	// int current_location_index = 0; // each server may have multiple locations; we use this so see which location in the list we are in

	bool	server_bracket_open = false;
	bool	location_bracket_open = false;

	Location	current_location;
	std::string		last_key;
	std::string key;


	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		last_key = key;
		// ss >> key;

		while (ss >> key)
		{
			if (key == "server" && !server_bracket_open)
			{
				// if (already_constructed)
				// 	_configs.push_back(current);
				current = ServerConfig();
				// already_constructed = true;
			}
			else if (key == "{")
			{
				if (last_key == "server")
					server_bracket_open = true;
				else if (last_key == "location")
				{
					location_bracket_open = true;
					parsing_location = true;
				}
			}
			else if (key == "}")
			{
				if (location_bracket_open)
				{
					location_bracket_open = false;
					parsing_location = false;
					current._locations.push_back(current_location);
				}
				else if (server_bracket_open)
				{
					_configs.push_back(current);
					server_bracket_open = false;
				}

			}
			else if (key == "listen")
			{
				ss >> current._port;
			}
			else if (key ==  "host")
			{
				ss >> current._host;
			}
			else if (key == "server_name")
			{
				ss >> current._server_name;
			}
			else if (key == "error_page")
			{
				std::string code;
				std::vector<std::string> temp;
				while (ss >> code)
					temp.push_back(code);
				int i = 0;
				while (i < temp.size() - 1)
				{
					current._error_pages[temp[i]] = temp.back();
					i++;
				}
			}
			else if (key == "client_max_body_size")
			{
				ss >> current._max_body;
			}
			else if (key == "return" && !location_bracket_open)
			{
				ss >> current._return;
			}
			else if (key == "root" && !location_bracket_open)
			{
				ss >> current._root;
			}
			else if (key == "index"  && !location_bracket_open)
			{
				ss >> current._index;
			}
			else if (key == "location" && !location_bracket_open)
			{
				// current._locations.clear();
				ss >> current_location._path;
				current_location = Location();
			}
			else if (parsing_location)	// we are inside the location block now
			{
				parse_location(key, ss, current_location); // 
			}
			last_key = key;
		}
	}

	if (server_bracket_open || location_bracket_open)
	{
		// error 
		// brackets are not properly formatted
		throw ConfigFileException();
	}

	
}

void	Config::parse_location(std::string &key, std::istringstream &ss, Location &current_location)
{
	if (key == "root")
	{
		ss >> current_location._root;
	}
	else if (key == "index")
		ss >> current_location._index;
	else if (key == "return")
		ss >> current_location._ret;
	else if (key == "allow_methods")
	{
		std::string method;
		while (ss >> method)
		{
			current_location._allowed_methods.push_back(method);
		}
	}
	else if (key == "autoindex")
	{
		std::string temp;
		ss >> temp;
		current_location._autoindex = temp == "on" ? true : false;
	}
	else if (key == "cgi_path")
	{
		std::string path;
		while (ss >> path)
		{
			current_location._cgi_path.push_back(path);
		}
	}
	else if (key == "cgi_ext")
	{
		std::string ext;
		while (ss >> ext)
		{
			current_location._cgi_ext.push_back(ext);
		}
	}
}

void Config::generate_servers(std::vector<Server> &servers)
{
	int i = 0;
	while (i < _configs.size())
	{
		Server server(_configs[i]);
		std::cout << "################################################################################" << std::endl;
		std::cout << "server " << i
		<< ": host: " << server.get_config()._host
		<< ", port: " << server.get_config()._port
		<<" generated."<< std::endl;
		servers.push_back(server);
		i++;
	}
	std::cout << "################################################################################" << std::endl;
	std::cout << std::endl;
}

// parameters that should be default
// host
// error page
// server name
// max request size
// index
// also throw exception if some mandatory parameters are not set
void	Config::init_if_not_set()
{
	int i = 0;
	while (i < _configs.size())
	{
		if (_configs[i]._port == "" || _configs[i]._root == "")
			throw ConfigFileException();
		if (_configs[i]._host == "")
			_configs[i]._host = "127.0.0.1";
		if (_configs[i]._server_name == "")
			_configs[i]._server_name = ""; // idk what to set here
		if (_configs[i]._index == "")
			_configs[i]._index = "index.html";
		if (_configs[i]._error_pages.empty())
			_configs[i]._error_pages["404"] = "public/error/404.html";
		if (_configs[i]._max_body == 0)
			_configs[i]._max_body = 1024;
		i++;
	}
}

const char * Config::ConfigFileException::what() const throw()
{
	return "Config file is not formatted properly, or not set properly";
}

std::vector<ServerConfig> &Config::get_configs()
{
	return _configs;
}
