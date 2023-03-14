#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <string>
#include <set>

#include <vector>
#include "src/Request.hpp"
#include "Response/Response.hpp"
#include "src/Config.hpp"
#include "src/Server.hpp"

void    check_incoming_connections(std::pair<fd_set, fd_set> &fds, std::vector<Server> &servers)
{
    fds = Server::wait_on_clients(servers);
	// std::cout << "select again" << std::endl;
    std::vector<Server>::iterator s = servers.begin();
    std::vector<Server>::iterator ite = servers.end();
    while (s != ite)
    {
        if (FD_ISSET(s->get_socket(), &fds.first)) // will return true if file descriptor was flagged by select
        {
            ClientInfo client;
            bzero(&client, sizeof(ClientInfo));
            client.address_length = sizeof(client.address);
			client.request = (char *) calloc(BASE_REQUEST_SIZE, sizeof(char));
			client.capacity = BASE_REQUEST_SIZE;
			client.still_receiving = false;
			client.still_saving = false;
			client.is_reading = false;
			client.fd = -1;
			client.received = 0;
            client.socket = accept(s->get_socket(),
                (struct sockaddr*) &(client.address),
                &(client.address_length));
			fcntl(client.socket, F_SETFL, O_NONBLOCK);
            client.last_received = time(NULL);
			s->clients.push_back(client);
			if (client.socket < 0) {
				std::cerr << "accept() failed. (" << errno << ") " << strerror(errno) << std::endl;
				exit(1);
			}
			std::cout << "######################################################" << std::endl;
			std::cout << "New connection from " << s->get_client_address(client)
				<< " : " << s->get_port() << " using socket " << client.socket << std::endl;
			std::cout << "####################################################" << std::endl;
        }
        s++;
    }
}

void    check_incoming_requests(std::pair<fd_set, fd_set> &fds, std::vector<Server> &servers, char** env)
{
    std::vector<Server>::iterator server = servers.begin();
    std::vector<Server>::iterator end = servers.end();
    while (server != end)
    {
        std::vector<ClientInfo>::iterator it = server->clients.begin();
        std::vector<ClientInfo>::iterator e = server->clients.end();
        while (it != e)
        {
			if (!it->still_receiving && it->is_reading && FD_ISSET(it->fd, &fds.first))
			{
				if (it->response.empty())
				{
					std::cout << "reading file" << std::endl;
					ssize_t  r;
					size_t byte_to_read = it->file_size - it->total_bytes_read;
					if (byte_to_read > CHUNK_SIZE_SEND)
						byte_to_read = CHUNK_SIZE_SEND;
					char s[byte_to_read];
					r = read(it->fd, s, byte_to_read);
					if (r < 0)
					{
						std::cout << "dropping client" << std::endl;
						if (it->fd != -1)
							close(it->fd);
						server->drop_client(*it);
						e = server->clients.end();
						continue ;
					}
					it->total_bytes_read += r;
					// std::cout << "total bytes read: " << r << std::endl;
					it->response = std::string(s, r);
					if (it->total_bytes_read == it->file_size)
					{
						// std::cout << "finished reading " << std::endl;
						it->is_reading = false;
						close(it->fd);
						it->fd = -1;
						it->total_bytes_read = 0;
					}
				}
				it->still_receiving = true;
			}
			else if (it->still_receiving && FD_ISSET(it->socket, &fds.second))
			{
				std::cout << "retrying receiving "<< std::endl;
				bool r = server->send_data(*it); // if true keep connection
				if (!r)
				{
						std::cout << "dropping client" << std::endl;
					if (it->fd != -1)
						close(it->fd);
				    it = server->drop_client(*it);
					e = server->clients.end();
                    continue ;
				}
			}
			else if (it->still_saving && FD_ISSET(it->fd, &fds.second))
			{
				std::cout << "savin file" << std::endl;
				ssize_t r;
				bool failed = false;
				size_t bytes_to_write = it->request_obj->_body_len - it->total_bytes_saved;
				if (bytes_to_write > CHUNK_SIZE_SAVE)
					bytes_to_write = CHUNK_SIZE_SAVE;
				r = write(it->fd, it->request_obj->_body.c_str() + it->total_bytes_saved,
					bytes_to_write);
				if (r < 0)
				{
						std::cout << "dropping client" << std::endl;
					if (it->fd != -1)
						close(it->fd);
					server->drop_client(*it);
					e = server->clients.end();
                    continue ;
				}
				it->total_bytes_saved += r;
				// std::cout << "bytes were saved: " << r << std::endl;
				if (it->total_bytes_saved == it->request_obj->_body_len)
				{
					it->still_saving = false;
					close(it->fd);
					it->fd = -1;
					it->total_bytes_saved = 0;
					it->still_receiving = true;
				}
			}
			else if (FD_ISSET(it->socket, &fds.first))
            {
                bool r = server->receive_request(it, env, fds); // return whether should close connection or not
				if (r)
                {
                    e = server->clients.end();
                    continue ;
                }
            }
			else
			{
                if (time(NULL) - it->last_received > TIMEOUT)
                {
                    std::cout << "timeout; socket fd :" << it->socket << std::endl;
                    it = server->drop_client(*it);
                    e = server->clients.end();
                    continue ;
                }
            }
            ++it;
        }
        ++server;
    }
}

int	main(int argc, char **argv, char **env)
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
	signal(SIGPIPE, SIG_IGN);
	std::vector<Server> servers;
	config.generate_servers(servers);
	Server::create_sockets(servers);
	while (1)
	{
		std::pair<fd_set, fd_set> fds;
		check_incoming_connections(fds, servers);
		check_incoming_requests(fds, servers, env);
	}
	return 0;
}
