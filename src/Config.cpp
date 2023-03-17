
#include "Config.hpp"


Config::Config(const std::string &file_path)
    : _file_path(file_path)
{
	_parsing_location = false;
	_server_bracket_open = false;
	_location_bracket_open = false;
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
	this->_file_path				= rhs._file_path;
	this->_configs					= rhs._configs;
	this->_parsing_location			= rhs._parsing_location;
	this->_server_bracket_open		= rhs._server_bracket_open;
	this->_location_bracket_open	= rhs._location_bracket_open;
	this->_current_location			= rhs._current_location;
	this->_last_key					= rhs._last_key;
	this->_key						= rhs._key;
	this->_current					= rhs._current;
	return (*this);
}

void	Config::print()
{
	std::vector<ServerConfig>::iterator it = _configs.begin();
	while (it != _configs.end())
	{
		std::cout << "server_name:		" << it->_server_name << std::endl;
		std::cout << "host: 			" << it->_host << std::endl;
		// std::cout << "index: " << it->_index << std::endl;
		for (int i = 0; i < it->_index.size(); i++)
			std::cout << "index: 			" << it->_index[i] << std::endl;
			
		std::cout << "root: 			" << it->_root << std::endl;
		std::cout << "max_body: 		" << it->_max_body << std::endl;
		std::cout << "auto_index: 		" << std::boolalpha << it->_auto_index << std::endl;
		std::map<int, std::string>::iterator err = it->_error_pages.begin();
		while (err != it->_error_pages.end())
		{
			std::cout << err->first << " 			" << err->second << std::endl;
			++err;
		}
		std::vector<Location>::iterator lo = it->_locations.begin();
		while (lo != it->_locations.end())
		{
			std::cout << "location: 			" << lo->_path << std::endl;
			std::cout << "	" << "_root: " << lo->_root << std::endl;
			std::cout << "	" << "_ret: " << lo->_ret.first << " " << lo->_ret.second << std::endl;
			std::cout << "	" << "_upload: " << std::boolalpha << lo->_upload << std::endl;
			std::cout << "	" << "_upload_dir: " << lo->_upload_dir << std::endl;
			std::cout << "	" << "_path: " << lo->_path << std::endl;
			for (int i = 0; i < lo->_index.size(); i++)
				std::cout << "	" << "index: " << lo->_index[i] << std::endl;
			std::cout << "	" << "auto_index: " << std::boolalpha << lo->_autoindex << std::endl;

			int i = 0;
			std::cout << "	allowed methods: " << std::endl;
			for (int i = 0; i < lo->_allowed_methods.size(); i++)
				std::cout << "	" << lo->_allowed_methods[i] << std::endl;
			std::cout << "	" << "_cgi_path: " << lo->_cgi_path << std::endl;
			++lo;
		}

		std::cout << "==========================================" << std::endl;
		++it;
	}
}

void	Config::parse_server(std::istringstream &ss)
{
	if (_server_bracket_open)
		throw ConfigFileException();
	_current = ServerConfig();
	_current._auto_index = false;
}

void	Config::parse_open_accolade(std::istringstream &ss)
{
	if (_last_key == "server")
		_server_bracket_open = true;
	else if (_last_key == "location")
	{
		_location_bracket_open = true;
		_parsing_location = true;
	}
	else
		throw ConfigFileException();
}

void	Config::parse_close_accolade(std::istringstream &ss)
{
	if (_location_bracket_open)
	{
		_location_bracket_open = false;
		_parsing_location = false;
		_current._locations.push_back(_current_location);
	}
	else if (_server_bracket_open)
	{
		_configs.push_back(_current);
		_server_bracket_open = false;
	}
	else
		throw ConfigFileException();
}

void	Config::parse_listen(std::istringstream &ss)
{
	if (!_server_bracket_open || _location_bracket_open)
		throw ConfigFileException();
	ss >> _current._port;
}

void	Config::parse_host(std::istringstream &ss)
{
	if (!_server_bracket_open || _location_bracket_open)
		throw ConfigFileException();
	ss >> _current._host;
}

void	Config::parse_server_name( std::istringstream &ss)
{
	if (!_server_bracket_open || _location_bracket_open)
		throw ConfigFileException();
	ss >> _current._server_name;
}

void	Config::parse_autoindex( std::istringstream &ss)
{
	if (!_server_bracket_open)
		throw ConfigFileException();
	std::string temp;
	bool		value;
	ss >> temp;
	if (temp == "on")
		value = true;
	else if (temp == "off")
		value = false;
	else
		throw ConfigFileException();
	if (_location_bracket_open)
	{
		_current_location._autoindex = value;
		_current_location._is_autoindex_set = true;
		return ;
	}
	_current._auto_index = value;

}

void	Config::parse_error_page( std::istringstream &ss)
{
	if (!_server_bracket_open)
		throw ConfigFileException();
	std::map<int, std::string>	*_error_pages;

	if (!_location_bracket_open)
		_error_pages = &_current._error_pages;
	else
		_error_pages = &_current_location._error_pages;
	int 		code;
	std::string	page;
	std::set<int> temp;
	while (ss >> code)
		temp.insert(code);
	ss.clear();
	ss >> page;
	std::set<int>::iterator it = temp.begin();
	while (it != temp.end())
	{
		(*_error_pages)[*it] = page;
		++it;
	}
}

void	Config::parse_client_max_body_size( std::istringstream &ss)
{
	if (!_server_bracket_open)
		throw ConfigFileException();
	if (_location_bracket_open)
		ss >> _current_location._max_body;
	else
		ss >> _current._max_body;
}

void	Config::parse_root( std::istringstream &ss)
{
	if (!_server_bracket_open)
		throw ConfigFileException();
	if (_location_bracket_open)
		ss >> _current_location._root;
	else
		ss >> _current._root;
}

void	Config::parse_index( std::istringstream &ss)
{
	if (!_server_bracket_open)
		throw ConfigFileException();
	std::vector<std::string>			*index;
	if (_location_bracket_open)
		index = &_current_location._index;
	else
		index = &_current._index;
	index->clear();
	std::string temp;
	while (ss >> temp)
		index->push_back(temp);
}

void	Config::parse_location( std::istringstream &ss)
{
	if (_location_bracket_open || !_server_bracket_open)
		throw ConfigFileException();	
	_current_location = Location();
	_current_location._max_body = -1;
	ss >> _current_location._path;
}

void	Config::parse_return( std::istringstream &ss)
{
	if (!_location_bracket_open || !_server_bracket_open)
		throw ConfigFileException();
	if (!(ss >> _current_location._ret.first))
		throw ConfigFileException();
	ss >> _current_location._ret.second;
}

void	Config::parse_methods( std::istringstream &ss)
{
	if (!_server_bracket_open)
		throw ConfigFileException();
	std::vector<std::string>	*allowed_methods;
	if (_location_bracket_open)
		allowed_methods = &_current_location._allowed_methods;
	else
		allowed_methods = &_current._allowed_methods;
	std::string method;
	allowed_methods->clear();
	while (ss >> method)
		allowed_methods->push_back(method);
}

void	Config::parse_cgi_path( std::istringstream &ss)
{
	if (!_server_bracket_open)
		throw ConfigFileException();
	if (!_location_bracket_open)
		ss >> _current._cgi_path;
	else
		ss >> _current_location._cgi_path;
}

void	Config::parse_upload( std::istringstream &ss )
{
	if (!_server_bracket_open)
		throw ConfigFileException();
	bool b;

	std::string temp;
	ss >> temp;
	if (temp == "on")
		b = true;
	else if (temp == "off")
		b = false;
	else
		throw ConfigFileException();
	if (!_location_bracket_open)
		throw ConfigFileException();
	else
		_current_location._upload = b;
}

void	Config::parse_upload_dir( std::istringstream &ss )
{
	if (!_server_bracket_open)
		throw ConfigFileException();
	if (!_location_bracket_open)
		throw ConfigFileException();
	else
		ss >> _current_location._upload_dir;
}

void    Config::parse()
{
   	std::ifstream 	file(_file_path);
	std::string		line;
	if (!file.is_open())
	{
		std::cerr << _file_path << ": error" << std::endl;
		exit (1);
	}

	typedef void (Config::*fun)(std::istringstream &ss);
	std::map<std::string, fun> handlers;
	handlers["server"]					= &Config::parse_server;
	handlers["{"]						= &Config::parse_open_accolade;
	handlers["}"]						= &Config::parse_close_accolade;
	handlers["listen"]					= &Config::parse_listen;
	handlers["host"]					= &Config::parse_host;
	handlers["server_name"]				= &Config::parse_server_name;
	handlers["autoindex"]				= &Config::parse_autoindex;
	handlers["error_page"]				= &Config::parse_error_page;
	handlers["client_max_body_size"]	= &Config::parse_client_max_body_size;
	handlers["root"]					= &Config::parse_root;
	handlers["index"]					= &Config::parse_index;
	handlers["location"]				= &Config::parse_location;
	handlers["return"]					= &Config::parse_return;
	handlers["allow_methods"]			= &Config::parse_methods;
	handlers["cgi_path"]				= &Config::parse_cgi_path;
	handlers["upload"]					= &Config::parse_upload;
	handlers["upload_dir"]				= &Config::parse_upload_dir;

	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		_last_key = _key;

		while (ss >> _key)
		{
			if (handlers.count(_key))
				(this->*(handlers[_key]))(ss);
			else if (_key[0] == '#')
			{
				// ignore
				while (ss >> _key);
				_key = "#";
			}
			else
				throw ConfigFileException();
			_last_key = _key;
		}
	}
	_key = "";
	if (_server_bracket_open || _location_bracket_open)
		throw ConfigFileException();
}

void Config::generate_servers(std::vector<Server> &servers)
{
	int i = 0;
	bool is_set;
	while (i < _configs.size())
	{
		is_set = false;
		for (int j = 0; j < i; j++)
		{
			if (servers[j].get_port() == _configs[i]._port && servers[j].get_host() == _configs[i]._host)
			{
				servers[j].add_config(_configs[i]);
				is_set = true;
				break ;
			}
		}
		if (!is_set)
		{
			Server server(_configs[i]);
			servers.push_back(server);	
		}
		i++;
	}
	// std::cout << "################################################################################" << std::endl;
	std::cout << std::endl;
}

bool	Config::is_port_valid(const std::string &port)
{
	int len = port.length();
	if (len == 0 || len > 5)
		return false ;
	for (int i = 0; i < len; i++)
	{
		if (!isdigit(port[i]))
			return false;
	}
	int num = std::stoi(port);
	if (num < 1 || num > 65535)
		return false;
	return true;
}

// should also check if port ranges from 1 to 65535
void	Config::init_if_not_set()
{
	int i = 0;
	while (i < _configs.size())
	{
		if (!is_port_valid(_configs[i]._port))
			throw ConfigFileException();
		if (_configs[i]._host == "")
			_configs[i]._host = "127.0.0.1";
		if (_configs[i]._server_name == "")
			_configs[i]._server_name = _configs[i]._host; // default server_name in ngnix is hostname
		if (_configs[i]._max_body == 0)
			_configs[i]._max_body = 1048576; // 1mb
		for (int j = 0; j < _configs[i]._locations.size(); j++)
		{
			if (!_configs[i]._locations[j]._is_autoindex_set)
				_configs[i]._locations[j]._autoindex = _configs[i]._auto_index;
			
		}
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


std::string const	&Config::get_key() const
{
	return this->_key;
}
