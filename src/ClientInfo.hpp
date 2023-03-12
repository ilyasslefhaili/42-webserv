

#ifndef CLIENT_INFO_HPP

# define CLIENT_INFO_HPP

# include "Request.hpp"

# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <errno.h>
# include <cstdio>
# include <string>
# include <cstring>

class Request;

	struct ClientInfo {
		socklen_t 				address_length;
		struct sockaddr_storage address;
		int						socket;
		char					*request;
		size_t					capacity;
		size_t					received;
		time_t 					last_received;
		std::string				response;
		ssize_t 				bytes_sent;
		ssize_t 				total_bytes_sent;
		Request					*request_obj;
		bool					still_receiving;

		bool					still_saving;
		int						fd;
		size_t					total_bytes_saved;

		// ClientInfo()
		// {
		// 	request_obj = nullptr;
		// }
		
	};

#endif