

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
#include <iostream>

#define BSIZE 1024
#define MAX_REQUEST_SIZE 2047

#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)

struct ClientInfo {
	socklen_t 				address_length;
	struct sockaddr_storage address;
	int						socket;
	char					request[MAX_REQUEST_SIZE + 1];
	int						received;
};


class Server {
	private:
		std::vector<ClientInfo> _clients;
		// other attributes to get form .config file

	public:
		Server();
		~Server();
		Server(const Server & server);
		Server & operator=(const Server & server);

		ClientInfo	&get_client(int socket);
		void		drop_client(ClientInfo & client);
		std::string	get_client_address(ClientInfo &client);
		fd_set		wait_on_clients(int server);
		void		send_400(ClientInfo &client);
		void		send_404(ClientInfo &client);
		void		serve_resource(ClientInfo &client, std::string path);
		int 		create_socket(const char* host, const char *port);

		std::vector<ClientInfo> &get_clients();

	public:
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