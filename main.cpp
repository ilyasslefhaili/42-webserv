#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)


#include <cstdio>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <string>

#include <vector>
#include "src/Request.hpp"
#include "Response/Response.hpp"
#include "src/Config.hpp"
#include "src/Server.hpp"



int	main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout << "usage: ./webserv 'path_to_config_file'" << std::endl;
		return (1);
	}
	Config config(argc == 2 ? argv[1] : "webserv.conf");
	try 
	{
		config.parse();
		config.init_if_not_set();
		// config.print(); 
	} catch (Config::ConfigFileException &e)
	{
		std::cout << e.what() << std::endl;
		exit(1);
	}
	std::vector<Server> servers;
	config.generate_servers(servers);


	// we test with one server first
	Server	server = servers[0];

	int server_socket = server.create_socket(server.get_config()._host.c_str(),
		server.get_config()._port.c_str());

	while (1)
	{
		fd_set reads;
 		reads = server.wait_on_clients(server_socket);
		if (FD_ISSET(server_socket, &reads)) // will return true if file descriptor was flagged by select
		{
			ClientInfo client;
			bzero(&client, sizeof(client));
			client.address_length = sizeof(client.address);
		
			// create a new socket for an incoming TCP connection.
			// accept blocks the program until a new connection is made
			client.socket = accept(server_socket,
				(struct sockaddr*) &(client.address),
				&(client.address_length)); 

			server.insert_client(client);
			
			if (!ISVALIDSOCKET(client.socket)) {
				fprintf(stderr, "accept() failed. (%d)\n",
					GETSOCKETERRNO());
				return 1;
			}
			std::cout << "New connection from " << server.get_client_address(client) << std::endl;

		}
		std::vector<ClientInfo>::iterator it = server.get_clients().begin();
		std::vector<ClientInfo>::iterator e = server.get_clients().end();
		while (it != e)
		{
			if (FD_ISSET(it->socket, &reads))
			{
				if (it->received == MAX_REQUEST_SIZE) {
					server.send_400(*it);
					continue;
				}
			}
			int r = recv(it->socket,
				it->request + it->received,
				MAX_REQUEST_SIZE - it->received, 0);
			if (r < 1) {
				std::cout << "Unexpected disconnect from " << server.get_client_address(*it) << std::endl;
				server.drop_client(*it);
			}
			else
			{
				it->received += r;
				it->request[it->received] = 0;

				// http header and body are seperated by a blank line
				// if q is not null we know tha the header has been received
				char *q = strstr(it->request, "\r\n\r\n");
				if (q)
				{
					Request request(it->request);
					server.serve_resource(*it, request);
				}
			}
			++it;
		}
	}
	printf("\nClosing socket...\n");
 	close(server_socket);
	printf("Finished.\n");
	return 0;
}



//dual sockets ?
// int option = 0;
// if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY,
// 	(void*)&option, sizeof(option))) {
// 		fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
// 		return 1;
// }