#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define GETSOCKETERRNO() (errno)


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <string>

#include "models/Server.hpp"

int	main()
{
	Server	server;

	int server_socket = server.create_socket("127.0.0.1", "8080");

	while (1)
	{
		fd_set reads;
 		reads = server.wait_on_clients(server_socket);
		if (FD_ISSET(server_socket, &reads))
		{
				ClientInfo client;
				client.address_length = sizeof(client.address);
				

			client.socket = accept(server_socket,
				(struct sockaddr*) &(client.address),
				&(client.address_length));

			server.insert_client(client);
			
			if (!ISVALIDSOCKET(client.socket)) {
				fprintf(stderr, "accept() failed. (%d)\n",
					GETSOCKETERRNO());
				return 1;
			}
			// printf("New connection from %s.\n",
			// server.get_client_address(client));
			std::cout << "New connection from " << server.get_client_address(client) << std::endl;

		}
		std::vector<ClientInfo>::iterator it = server.get_clients().begin();
		std::vector<ClientInfo>::iterator e = server.get_clients().end();
		while (it != e)
		{
			if (FD_ISSET(it->socket, &reads))
			{
				if (MAX_REQUEST_SIZE == it->received) {
					server.send_400(*it);
					continue;
				}
			}
			int r = recv(it->socket,
				it->request + it->received,
				MAX_REQUEST_SIZE - it->received, 0);
			if (r < 1) {
				// printf("Unexpected disconnect from %s.\n",
				// server.get_client_address(*it));
				std::cout << "Unexpected disconnect from " << server.get_client_address(*it) << std::endl;
				server.drop_client(*it);
			}
			else
			{
				it->received += r;
				it->request[it->received] = 0;
				char *q = strstr(it->request, "\r\n\r\n");
				if (q)
				{
					std::cout<<it->request<<std::endl;
					if (strncmp("GET /", it->request, 5)) {
						server.send_400(*it);
					}
					else
					{
						char *path = it->request + 4;
						char *end_path = strstr(path, " ");
						if (!end_path) {
							server.send_400(*it);
						}
						else
						{
							*end_path = 0;
							server.serve_resource(*it, path);
						}
 					}
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