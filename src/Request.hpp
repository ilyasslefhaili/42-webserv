

#ifndef REQUEST_HPP

# define REQUEST_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>


class Request {

	public:
		std::string							_raw;
		std::string							_method;
		std::string							_protocol_ver;	//ver of HTTP protocol
		std::map<std::string, std::string>	_header;
		std::string							_body;
		std::string 						_path;
		// SESSION ?


	private:
		void		get_method_and_path(std::string &line);

	public:
		Request(const char *);
		~Request();

		void		parse_request(const char *);
		void   		print_request() const;
		static bool request_is_complete(const char* buffer, int length);



};


#endif