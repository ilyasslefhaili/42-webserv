

#ifndef SERVER_HPP

# define SERVER_HPP

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

# define BSIZE 1024
# define MAX_REQUEST_SIZE 4096

// #define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)

# include "Location.hpp"
# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include "../Response/Response.hpp"
# include "Request.hpp"
# include "Config.hpp"
# include "ServerConfig.hpp"
# include <set>
#include <ctime>

struct ClientInfo {
	socklen_t 				address_length;
	struct sockaddr_storage address;
	int						socket;
	char					request[MAX_REQUEST_SIZE + 1];
	int						received;
	time_t 					last_received;

	// std::string				buffer; // use this to remplace request
	
};

struct ServerConfig;

class Server {
	private:
		std::vector<ClientInfo> 			_clients;
		ServerConfig						_config;
		int									_socket; // socket used for listening

	public:
		// Server();
		Server(ServerConfig const &config);
		~Server();
		Server(const Server & server);
		Server & operator=(const Server & server);

		// takes a socket and search _clients
		ClientInfo	get_client(int socket);

		// closes the connection to a client and removes it from _clients
		std::vector<ClientInfo>::iterator		drop_client(ClientInfo & client);

		// returns a clients' ip address
		std::string	get_client_address(ClientInfo &client);

		// wait until either a client has data available or a new client is attempting to connect
		// using the function select		
		static fd_set		wait_on_clients(std::set<int> const &sockets,  std::vector<Server>  &servers);
		static std::set<int>	create_sockets(std::vector<Server> &servers);
		static void	ack_client(std::vector<Server> &servers, int socket, ClientInfo &client);
		// fd_set				wait_on_clients(int server);


		// handle http error condition
		std::vector<ClientInfo>::iterator		send_400(ClientInfo &client);
		std::vector<ClientInfo>::iterator		send_404(ClientInfo &client);

		// transfer a file to a connected client
		bool		serve_resource(ClientInfo &client, Request &request, std::vector<ServerConfig> &configs);

		// creates a socket for listening
		int 		create_socket(const char* host, const char *port);


		std::vector<ClientInfo> &get_clients();
		void					insert_client(ClientInfo &client);


		ServerConfig	& get_config();
		int				get_socket();
		void			set_socket(int socket);


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