

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
		
		size_t 					total_bytes_sent;
		Request					*request_obj;
		bool					is_receiving;

		bool					is_saving;
		std::string				file_name;
		int						fd;
		size_t					total_bytes_saved;

		bool					is_reading;
		size_t					total_bytes_read;
		size_t					file_size;
		
		bool					force_drop_connection;

		bool					chunk_finished;
		bool					first_time;
		size_t					chunk_size;
		std::string				body;
		std::string				new_body;

		bool 					header_reached;
		bool					body_len_check;
	};

#endif