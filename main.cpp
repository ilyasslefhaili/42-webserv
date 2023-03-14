# include "main.hpp"

void    check_incoming_connections(std::pair<fd_set, fd_set> &fds, std::vector<Server> &servers);

void    check_incoming_requests(std::pair<fd_set, fd_set> &fds, std::vector<Server> &servers, char** env);

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
