#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

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


void    check_incoming_connections(fd_set &reads, std::vector<Server> &servers)
{
    reads = Server::wait_on_clients(servers);
    std::vector<Server>::iterator s = servers.begin();
    std::vector<Server>::iterator ite = servers.end();
    while (s != ite)
    {
        if (FD_ISSET(s->get_socket(), &reads)) // will return true if file descriptor was flagged by select
        {
            ClientInfo client;
            bzero(&client, sizeof(ClientInfo));
            client.address_length = sizeof(client.address);
            client.socket = accept(s->get_socket(),
                (struct sockaddr*) &(client.address),
                &(client.address_length));
			int reuseaddr = 1;
			setsockopt(s->get_socket(), SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));

            client.last_received = time(NULL);
			s->insert_client(client);
			if (client.socket < 0) {
				fprintf(stderr, "accept() failed. (%d)\n",
					errno);
				exit(1);
			}
			std::cout << "###############################" << std::endl;
			std::cout << "New connection from " << s->get_client_address(client)
				<< " : " << s->get_port() << " using socket " << client.socket << std::endl;
        }
        s++;
    }
}

void    check_incoming_requests(fd_set &reads, std::vector<Server> &servers)
{
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
				for(int i = 0; i < server->get_clients().size(); i++)
				{
					std::cout << " client " << i << " socket : " << server->get_clients()[i].socket << std::endl;
				}
                if (server->receive_request(it))
                {
                    e = server->get_clients().end();
                    continue ;
                }
            }
			else
			{
                // check timeout
                if (time(NULL) - it->last_received > TIMEOUT)
                {
                    std::cout << "timeout; socket with fd " << it->socket << std::endl;
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
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		if (config.get_key() != "")
			std::cout << "invalid id " << config.get_key() << std::endl;
		exit(1);
	}
	std::vector<Server> servers;
	config.generate_servers(servers);
	Server::create_sockets(servers);
	while (1)
	{
		fd_set reads;
		check_incoming_connections(reads, servers);
		check_incoming_requests(reads, servers);
	}
	return 0;
}
