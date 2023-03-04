#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define GETSOCKETERRNO() (errno)


#include <cstdio>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <string>
#include <set>

#include <vector>
#include "src/Request.hpp"
#include "Response/Response.hpp"
#include "src/Config.hpp"
#include "src/Server.hpp"

# define TIMEOUT 30

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
	}
	catch (Config::ConfigFileException &e)
	{
		std::cout << e.what() << std::endl;
		exit(1);
	}
	std::vector<Server> servers;
	config.generate_servers(servers);

	// first thing we loop through our servers and create socket for each one 
	std::set<int> sockets = Server::create_sockets(servers);

	while (1)
	{
		fd_set reads;
 		reads = Server::wait_on_clients(sockets, servers);
		std::set<int>::iterator s = sockets.begin();
		std::set<int>::iterator ite = sockets.end();
		while (s != ite)
		{
			if (FD_ISSET(*s, &reads)) // will return true if file descriptor was flagged by select
			{
				ClientInfo client;
				bzero(&client, sizeof(client));
				client.address_length = sizeof(client.address);
				// create a new socket for an incoming TCP connection.
				// accept blocks the program until a new connection is made
				client.socket = accept(*s,
					(struct sockaddr*) &(client.address),
					&(client.address_length));
				client.last_received = time(NULL);
				Server::ack_client(servers, *s, client);
			}
			s++;
		}
		std::vector<Server>::iterator server = servers.begin();
		std::vector<Server>::iterator end = servers.end();
		while (server != end)
		{
			std::vector<ClientInfo>::iterator it = server->get_clients().begin();
			std::vector<ClientInfo>::iterator e = server->get_clients().end();
			while (it != e)
			{
				if (FD_ISSET(it->socket, &reads))
				{
					if (it->received == MAX_REQUEST_SIZE) {
						it = server->send_400(*it);
						e = server->get_clients().end();
						continue ;
					}
					int r = recv(it->socket,
						it->request + it->received,
						MAX_REQUEST_SIZE - it->received, 0);
					if (r < 1) {
						std::cout << "Unexpected disconnect from " << server->get_client_address(*it) << std::endl;
						it = server->drop_client(*it);
						e = server->get_clients().end();
						continue ;
					}
					else
					{
						it->last_received = time(NULL);
						it->received += r;
						it->request[it->received] = 0;

						if (Request::request_is_complete(it->request, it->received)) // true if request is fully received; start processing
						{
							Request request(it->request);
							if (!server->serve_resource(*it, request, config.get_configs()))
							{
						 		it = server->drop_client(*it);
								e = server->get_clients().end();
								continue ;
							}
						}
					}
				} else {
					// check timeout
					if (time(NULL) - it->last_received > TIMEOUT)
					{
						std::cout << "timeout; socket " << it->socket << std::endl;
						it = server->drop_client(*it);
						e = server->get_clients().end();
						continue ;
					}
				}
				++it;
			}
			++server;
		}
	}

	// printf("\nClosing socket...\n");
 	// close(server.get_socket());
	// printf("Finished.\n");
	return 0;
}



//dual sockets ?
// int option = 0;
// if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY,
// 	(void*)&option, sizeof(option))) {
// 		fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
// 		return 1;
// }