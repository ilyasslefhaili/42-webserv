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

# define TIMEOUT 5

void    check_incoming_connections(fd_set &reads, std::set<int> &sockets, std::vector<Server> &servers)
{
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
            client.socket = accept(*s,
                (struct sockaddr*) &(client.address),
                &(client.address_length));
            client.last_received = time(NULL);
            Server::ack_client(servers, *s, client);
        }
        s++;
    }
}

void    check_incoming_requests(fd_set &reads, std::vector<Server> &servers, Config &config)
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
                if (server->receive_request(it, config))
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
	std::set<int> sockets = Server::create_sockets(servers);

	while (1)
	{
		fd_set reads;
		check_incoming_connections(reads, sockets, servers);
		check_incoming_requests(reads, servers, config);
	}
	return 0;
}



//dual sockets ?
// int option = 0;
// if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY,
// 	(void*)&option, sizeof(option))) {
// 		fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
// 		return 1;
// }