
#include "Server.hpp"

Server::Server(ServerConfig const &config)
{
	_configs.push_back(config);
	_port = config._port;
	_host = config._host;
}

Server::~Server()
{
	
}

Server::Server(const Server & server)
{
	*this = server;
}

Server & Server::operator=(const Server & server)
{
	this->clients = server.clients;
	this->_configs = server._configs;
	this->_socket = server._socket;
	this->_port = server._port;
	this->_host = server._host;
	return *this;
}

std::vector<ClientInfo>::iterator Server::drop_client(ClientInfo & client)
{
	std::cout << "closing socket: " << client.socket << std::endl;
	close(client.socket);
	std::vector<ClientInfo>::iterator it = clients.begin();
	while (it != clients.end())
	{
		if (client.socket == it->socket)
		{
			delete it->request_obj;
			free(it->request);
			return (clients.erase(it));
		}
		++it;
	}
	return (clients.end());
	// throw client not found ?
}

std::string	Server::get_client_address(ClientInfo &client)
{
	char address_buffer[100];

	getnameinfo((struct sockaddr*)&client.address,
		client.address_length,
		address_buffer,
		sizeof(address_buffer), 0, 0,
		NI_NUMERICHOST);
	return (std::string(address_buffer));
}


std::pair<fd_set, fd_set>  Server::wait_on_clients(std::vector<Server>  &servers)
{
	fd_set	reads;// a struct which will hold all our active sockets
	fd_set	writes;
	FD_ZERO(&reads);// Initialize fd_set reads to have zero bits for all file descriptors.
	FD_ZERO(&writes);
	int max_socket = -1; // this var will always have the largest socket fd

	std::vector<Server>::iterator serv = servers.begin();
	while (serv != servers.end())
	{
		FD_SET(serv->_socket, &reads);
		// FD_SET(serv->_socket, &writes);
		if (serv->get_socket() > max_socket)
			max_socket = serv->get_socket();
		std::vector<ClientInfo>::iterator cl = serv->clients.begin();
		while (cl != serv->clients.end())
		{
			if (!cl->is_receiving)
				FD_SET(cl->socket, &reads);
			else
				FD_SET(cl->socket, &writes);
			if (cl->socket > max_socket)
				max_socket = cl->socket;
			if (cl->fd != -1)
			{
				if (cl->is_reading)
					FD_SET(cl->fd, &reads);
				if (cl->is_saving)
					FD_SET(cl->fd, &writes);
				if (cl->fd > max_socket)
					max_socket = cl->fd;
			}
			++cl;
		}
		++serv;
	}
	struct timeval timeout;
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;

	// select indicates which of the specified file descriptors is ready for reading, ready for writing, or has an error condition pending
	if (select(max_socket + 1, &reads, &writes, 0, &timeout) < 0) {
		std::cerr << "select() failed. (" << errno << ") " << strerror(errno) << std::endl;
		
		exit(1);
		// throw ? exceptions to-do
	}
	return std::make_pair(reads, writes);
}

void	Server::create_sockets(std::vector<Server> &servers)
{
	size_t i = 0;

	while (i < servers.size())
	{
		servers[i].create_socket(servers[i].get_host().c_str(),
				servers[i].get_port().c_str());
		i++;
	}
}

void	Server::send_400(ClientInfo &client)
{
	const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
		"Connection: close\r\n"
		"Content-Length: 11\r\n\r\nBad Request";
		client.is_receiving = true;
	client.response = std::string(c400);
	give_error_page(*client.request_obj, this->_configs, 400);
	client.force_drop_connection = true;

	// send(client.socket, c400, strlen(c400), 0);

}

void	Server::send_413(ClientInfo &client)
{
	const char *c413 = "HTTP/1.1 413 Request Entity Too Large\r\n"
		"Connection: close\r\n"
		"Content-Length: 24\r\n\r\nRequest Entity Too Large";
	client.response = std::string(c413);
	give_error_page(*client.request_obj, this->_configs, 413);
	client.force_drop_connection = true;
	
	// send(client.socket, c413, strlen(c413), 0);

}

void	Server::send_404(ClientInfo &client)
{
	const char *c404 = "HTTP/1.1 404 Not Found\r\n"
		"Connection: close\r\n"
		"Content-Length: 9\r\n\r\nNot Found";
	client.response = std::string(c404);
	client.force_drop_connection = true;

	give_error_page(*client.request_obj, this->_configs, 404);
	// send(client.socket, c404, strlen(c404), 0);
}

void	Server::send_500(ClientInfo &client)
{
	const char *c500 = "HTTP/1.1 500 Internal Server Error\r\n"
		"Connection: close\r\n"
		"Content-Length: 21\r\n\r\nInternal Server Error";
	client.is_receiving = true;
	client.force_drop_connection = true;
	client.response = std::string(c500);
	give_error_page(*client.request_obj, this->_configs, 500);
	// send(client.socket, c404, strlen(c404), 0);
}



void	reset_req(ClientInfo &client)
{
	delete client.request_obj;
	client.request_obj = nullptr;
	free(client.request);
	client.request = (char *) malloc(BASE_REQUEST_SIZE * sizeof(char));
	client.capacity = BASE_REQUEST_SIZE;
	client.received = 0;
	client.header_reached = false;
	client.body_len_check = false;
	client.chunk_finished = true;
	client.total_bytes_sent = 0;
	client.first_time = true;
	client.chunk_size = 0;
	client.response = "";
}

bool		Server::send_data(ClientInfo &client)
{

	size_t bytes_to_send = client.response.size() - client.total_bytes_sent;
	if (bytes_to_send > CHUNK_SIZE_SEND)
		bytes_to_send = CHUNK_SIZE_SEND;
	ssize_t bytes_sent = send(client.socket, client.response.c_str() + client.total_bytes_sent,
		bytes_to_send, 0);
	if (bytes_sent < 1)
	{
		if (bytes_sent == 0)
			std::cout << "Connection was closed by the other end" << std::endl;
		else
		{
			std::cout << "Unexpected disconnect from " << this->get_client_address(client) << std::endl;
			std::cerr << strerror(errno) << std::endl;
		}
		return false ;
	}
	// std::cout << bytes_sent << " bytes were sent " << std::endl;
	client.last_received = time(NULL);
	client.total_bytes_sent += bytes_sent;
	// }
	if (client.total_bytes_sent != client.response.size())
	{
		client.is_receiving = true;
		return (true);
	}
	client.is_receiving = false;
	if (client.is_reading)
	{
		client.response.clear();
		client.total_bytes_sent = 0;
		return (true);
	}
	// std::cout << "client " << client.socket << " received " << client.total_bytes_sent << " bytes." << std::endl;
	std::string connection = client.request_obj->_header["Connection"];
	reset_req(client);
	if (client.force_drop_connection)
		return false;
	if (connection == "keep-alive")
	{
		// std::cout << "keeping the connection alive" << std::endl;
		return true;
	}
	else
	{
		// std::cout << "closing the connection " << std::endl;
		return false;
	}
}

// returns whether the connection should be open or not
bool		Server::serve_resource(ClientInfo &client)
{
	// std::cout << "server_resource " << get_client_address(client) << " " << client.request_obj->_path << std::endl;
	client.response = get_response(*client.request_obj, _configs);
	// std::cout << client.response << std::endl;
	client.total_bytes_sent = 0;
	if (client.is_saving)
		return (true);
	client.is_receiving = true;
	return (true);
	// if (!FD_ISSET(client.socket, &fds.second))
	// {
	// 	std::cout << "not yet, we shall wait" << std::endl;
	//  	return (true);
	// }
	// return (send_data(client));

}

// returns true if clients dropped
bool			Server::receive_request(std::vector<ClientInfo>::iterator &it, char **env)
{
	if (it->received == it->capacity)
	{
		size_t new_capacity;
		if (it->header_reached && it->request_obj != nullptr)
		{
			new_capacity = it->request_obj->buffer_size;
		}
		else
			new_capacity = it->capacity * 2;
		if (new_capacity > MAX_REQUEST_SIZE || new_capacity < it->capacity)
		{
			send_413(*it);
			it->force_drop_connection = true;
			it->is_receiving = true;
			return false ;
		}
		it->capacity = new_capacity;
		char *temp = (char *) malloc(sizeof(char) * it->capacity);
		if (!temp)
		{
			send_500(*it);
			it->force_drop_connection = true;
			it->is_receiving = true;
			return false ;
		}
		memcpy(temp, it->request, it->received);
		free(it->request);
		it->request = temp;
    }
	int r = recv(it->socket,
		it->request + it->received,
		it->capacity - it->received, 0);
	if (r < 1)
	{
		if (r == 0)
			std::cout << "Connection was closed by the other end" << std::endl;
		else
		{
			std::cout << "Unexpected disconnect from " << this->get_client_address(*it) << std::endl;
			std::cerr << strerror(errno) << std::endl;
		}
		it = this->drop_client(*it);
		return true ;
	}
	else
	{
		it->last_received = time(NULL);
		it->received += r;
		if (Request::request_is_complete(it->request, it->received, r, *it)) // true if request is fully received; start processing
		{
			// std::cout <<  it->received << " total bytes received from client: " << it->socket  << std::endl;
			if (it->request_obj != nullptr)
				delete it->request_obj;
			it->request_obj = new Request(it->request, it->received, *it);
			it->request_obj->_env = env;
			if (!this->serve_resource(*it))
			{
			    it = this->drop_client(*it);
			    return true ;
			}
		}
		else if (it->header_reached && !it->body_len_check)
		{
			it->body_len_check = true;
			if (it->request_obj != nullptr && !check_body_size(_configs, *it->request_obj))
			{
				std::cout << " true " << std::endl;

				send_413(*it);
				it->force_drop_connection = true;
				it->is_receiving = true;
				return false ;
			}
		}
	}
	return false ;
}


int Server::create_socket(const char* host, const char *port)
{
	std::cout << "Configuring local address......" << std::endl;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	struct addrinfo *bind_address;
	int status = getaddrinfo(host, port, &hints, &bind_address);
	if (status != 0)
	{
		std::cerr << "getaddrinfo() failed. (" << errno << ") " << gai_strerror(errno) << std::endl;
		exit(1);
	}
	std::cout << "Creating socket... " << host << " : " << port << std::endl;
	int socket_listen;
	socket_listen = socket(bind_address->ai_family,
	bind_address->ai_socktype, bind_address->ai_protocol);
	if (socket_listen < 0)
	{
		std::cerr << "socket() failed. (" << errno << ") " << strerror(errno) << std::endl;
		exit(1);
	}
	std::cout << "Binding socket to local address..." << std::endl;
	if (bind(socket_listen,
		bind_address->ai_addr, bind_address->ai_addrlen))
	{
		std::cerr << "bind() failed. (" << errno << ") " << strerror(errno) << std::endl;
		exit(1);
	}
	freeaddrinfo(bind_address);
	std::cout << "Listening to port " << port << std::endl;
	int optval = 1;
	if (setsockopt(socket_listen, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		std::cerr << "setsockopt() failed. (" << errno << ") " << strerror(errno) << std::endl;
        return 1;
    }
	if (listen(socket_listen, SOMAXCONN) < 0)
	{
		std::cerr << "listen() failed. (" << errno << ") " << strerror(errno) << std::endl;
		exit(1);
	}
	std::cout << std::endl;
	_socket = socket_listen;
	return socket_listen;
}

std::vector<ServerConfig>	&Server::get_configs()
{
	return _configs;
}

void	Server::add_config(ServerConfig const &config)
{
	_configs.push_back(config);
}


int		Server::get_socket() const
{
	return _socket;
}

void			Server::set_socket(int socket)
{
	_socket = socket;
}


std::string			Server::get_port() const
{
	return _port;
}

void				Server::set_port(std::string &port)
{
	_port = port;
}

std::string			Server::get_host() const
{
	return this->_host;
}

void	Server::set_host(std::string &host)
{
	this->_host = host;
}
