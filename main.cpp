# include "main.hpp"

void    check_incoming_connections(std::pair<fd_set, fd_set> &fds, std::vector<Server> &servers);

void    check_incoming_requests(std::pair<fd_set, fd_set> &fds, std::vector<Server> &servers, char** env);

std::vector<Server> *g_servers = nullptr;

void	handle_term(int sign)
{
	(void) sign;
	if (g_servers != nullptr)
	{
		std::vector<Server>::iterator serv = g_servers->begin();
		while (serv != g_servers->end())
		{
			close(serv->get_socket());
			std::vector<ClientInfo>::iterator cl = serv->clients.begin();
			while (cl != serv->clients.end())
			{
				close(cl->socket);
				if (cl->fd != -1)
					close(cl->fd);
				++cl;
			}
			++serv;
		}
	}
	exit(0);
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
	signal(SIGINT, handle_term);
	std::vector<Server> servers;
	config.generate_servers(servers);
	Server::create_sockets(servers);
	g_servers = &servers;
	while (1)
	{
		std::pair<fd_set, fd_set> fds;
		check_incoming_connections(fds, servers);
		check_incoming_requests(fds, servers, env);
	}
	return 0;
}
