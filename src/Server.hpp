

#ifndef SERVER_HPP

# define SERVER_HPP

# include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <string>

#define BSIZE 1024
#define MAX_REQUEST_SIZE 2047

// #define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)

#include "Location.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "../Response/Response.hpp"
#include "Request.hpp"


struct ClientInfo {
	socklen_t 				address_length;
	struct sockaddr_storage address;
	int						socket;
	char					request[MAX_REQUEST_SIZE + 1];
	int						received;
};


class Server {
	private:
		std::vector<ClientInfo> 			_clients;
		std::string							_port;
		std::string							_server_name;
		std::string							_root;
		std::string							_index;
		
		std::vector<Location>				_locations;

		// the key is gonna be the response code, and the value is the error page
		std::map<std::string, std::string>	_error_pages;

		// other attributes to get form .config file

	public:
		Server();
		~Server();
		Server(const Server & server);
		Server & operator=(const Server & server);

		void	add_location(Location &location);

		// takes a socket and search _clients
		ClientInfo	get_client(int socket);

		// closes the connection to a client and removes it from _clients
		void		drop_client(ClientInfo & client);

		// returns a clients' ip address
		std::string	get_client_address(ClientInfo &client);

		// wait until either a client has data available or a new client is attempting to connect
		// using the function select
		fd_set		wait_on_clients(int server);

		// handle http error condition
		void		send_400(ClientInfo &client);
		void		send_404(ClientInfo &client);

		// transfer a file to a connected client
		void		serve_resource(ClientInfo &client, Request &request);

		// creates a socket for listening
		int 		create_socket(const char* host, const char *port);


		std::vector<ClientInfo> &get_clients();
		void					insert_client(ClientInfo &client);


		// a functor, so we can use some algorithm functions.
		class MatchSocket
		{
			private:
				int	socket;
			public:
				MatchSocket(int s);
				bool operator()(const ClientInfo& obj) const;
		};

};


#endif