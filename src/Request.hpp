

#ifndef REQUEST_HPP

# define REQUEST_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>
#include "ClientInfo.hpp"


class Request {

	public:
		std::string							_raw;
		char**								_env;
		std::string							_method;
		std::string							_protocol_ver;	//ver of HTTP protocol
		std::map<std::string, std::string>	_header;
		std::string							_body;
		// char								*_body;
		size_t								_body_len;
		std::string 						_path;
		ClientInfo							&_client;
		bool								_is_chunked;
		bool								_header_only;
		size_t								buffer_size;


	private:
		void		get_method_and_path(std::string &line);

	public:
		Request(ClientInfo &client);
		Request(const char *request, size_t length, ClientInfo &client);
		Request(ClientInfo &client, const char *header, size_t length);
		Request(const Request &src);
		Request & operator=(const Request &rhs);
		~Request();

		void		parse_request(const char *request, size_t length);
		void   		print_request() const;
		static bool request_is_complete(const char* buffer, size_t length, int added_length, ClientInfo &client);
		void		set_request(const char *request, size_t length);



};


#endif