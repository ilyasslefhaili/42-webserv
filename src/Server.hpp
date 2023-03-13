

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
# include <cstring>

# define BSIZE 1024
# define BASE_REQUEST_SIZE 10000000
# define TIMEOUT 60
# define CHUNK_SIZE 10000000
# define MAX_REQUEST_SIZE 1000000000

# include "Location.hpp"
# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include "../Response/Response.hpp"
# include "Request.hpp"
# include "Config.hpp"
# include "ServerConfig.hpp"
# include "ClientInfo.hpp"
# include <set>
# include <ctime>
# include <utility>


struct	ServerConfig;
class	Config;

class Server {
	private:
		std::vector<ServerConfig>			_configs;
		int									_socket;
		std::string							_port;
		std::string							_host;

	public:

		std::vector<ClientInfo> 			clients;

		// Server();
		Server(ServerConfig const &config);
		~Server();
		Server(const Server & server);
		Server & operator=(const Server & server);

		// closes the connection to a client and removes it from _clients
		std::vector<ClientInfo>::iterator		drop_client(ClientInfo & client);

		// returns a clients' ip address
		std::string								get_client_address(ClientInfo &client);

		// wait until either a client has data available or a new client is attempting to connect
		// using the function select		
		static std::pair<fd_set, fd_set>  		wait_on_clients(std::vector<Server>  &servers);
		static void								create_sockets(std::vector<Server> &servers);
		// static void								ack_client(std::vector<Server> &servers, int socket, ClientInfo &client);
		// fd_set				wait_on_clients(int server);


		// handle http error condition
		void									send_400(ClientInfo &client);
		void									send_404(ClientInfo &client);
		void 									send_413(ClientInfo &client);


		// transfer a file to a connected client
		bool									serve_resource(ClientInfo &client,
												std::pair<fd_set, fd_set> &fds);

		bool									send_data(ClientInfo &client);
		

		// creates a socket for listening
		int 									create_socket(const char* host, const char *port);


		void									add_config(ServerConfig const &config);
		std::vector<ServerConfig>				&get_configs();
		int										get_socket() const;
		void									set_socket(int socket);
		std::string								get_port() const;
		void									set_port(std::string &port);
		std::string								get_host() const;
		void									set_host(std::string &host);

		bool									receive_request(std::vector<ClientInfo>::iterator &it,
												char **env, std::pair<fd_set, fd_set> &fds);
};


#endif