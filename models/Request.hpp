

#ifndef REQUEST_HPP

# define REQUEST_HPP

#include <string>
#include <map>



class Request {

	public:
		std::string							_method;
		std::string							_url;
		std::string							_protocol_ver;	//ver of HTTP protocol
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_remote_address; // address + port maybe change later
		std::map<std::string, std::string>	_cookies;
		std::map<std::string, std::string>	_query_params;
		std::string 						_path;
		// SESSION ?


	// public:
		// Request();
		// ~Request();


};


#endif