
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

// static 
fd_set  Server::wait_on_clients(std::vector<Server>  &servers)
{
	fd_set	reads;// a struct which will hold all our active sockets
	FD_ZERO(&reads);// Initialize fd_set reads to have zero bits for all file descriptors.
	int max_socket = -1; // this var will always have the largest socket fd


	std::vector<Server>::iterator serv = servers.begin();
	while (serv != servers.end())
	{
		FD_SET(serv->_socket, &reads);
		if (serv->get_socket() > max_socket)
			max_socket = serv->get_socket();
		std::vector<ClientInfo>::iterator cl = serv->clients.begin();
		while (cl != serv->clients.end())
		{
			FD_SET(cl->socket, &reads);
			if (cl->socket > max_socket)
				max_socket = cl->socket;
			++cl;
		}
		++serv;
	}

	struct timeval timeout;
	timeout.tv_sec = TIMEOUT - 1;
	timeout.tv_usec = 0;

	// select indicates which of the specified file descriptors is ready for reading, ready for writing, or has an error condition pending
	if (select(max_socket + 1, &reads, 0, 0, &timeout) < 0) {
		std::cerr << "select() failed. (" << errno << ") " << strerror(errno) << std::endl;
		
		exit(1);
		// throw ? exceptions to-do
	}
	return reads;
}

void	Server::create_sockets(std::vector<Server> &servers)
{
	int i = 0;

	while (i < servers.size())
	{
		servers[i].create_socket(servers[i].get_host().c_str(),
				servers[i].get_port().c_str());
		i++;
	}
}

std::vector<ClientInfo>::iterator Server::send_400(ClientInfo &client)
{
	const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
		"Connection: close\r\n"
		"Content-Length: 11\r\n\r\nBad Request";
	send(client.socket, c400, strlen(c400), 0);
	return drop_client(client);
}

std::vector<ClientInfo>::iterator Server::send_404(ClientInfo &client)
{
	const char *c404 = "HTTP/1.1 404 Not Found\r\n"
		"Connection: close\r\n"
		"Content-Length: 9\r\n\r\nNot Found";
	send(client.socket, c404, strlen(c404), 0);
	return drop_client(client);
}
# define CHUNK_SIZE 4096
// returns whether the connection should be open or not
bool		Server::serve_resource(ClientInfo &client, Request &request)
{
	std::cout << "server_resource " << get_client_address(client) << " " << request._path << std::endl;
	if (request._path.size() > 100)
	{
		send_400(client);
		return false;
	}
	if (request._path.find("..") != std::string::npos)
	{
		send_404(client);
		return false;
	}
	client.response = get_response(request, _configs);
	client.bytes_sent = 0;
	client.total_bytes_sent = 0;
	while (client.total_bytes_sent < client.response.size())
	{
		int bytes_to_send = client.response.size() - client.total_bytes_sent;
		if (bytes_to_send > CHUNK_SIZE)
			bytes_to_send = CHUNK_SIZE;
		client.bytes_sent = send(client.socket, client.response.c_str() + client.total_bytes_sent,
			bytes_to_send, 0);
		if (client.bytes_sent < 0)
		{
			std::cerr << "error (" << errno << ") " << strerror(errno) << std::endl; // forbidden, just for testing
			return false ;
		}
		std::cout << client.bytes_sent << " bytes were sent " << std::endl;
		client.total_bytes_sent += client.bytes_sent;
	}
	std::cout << "client " << client.socket << " received " << client.total_bytes_sent << " bytes." << std::endl;

	if (request._header["Connection"] == "keep-alive")
	{
		std::cout << "keeping the connection alive" << std::endl;
		free(client.request);
		client.request = (char *) calloc(BASE_REQUEST_SIZE, sizeof(char));
		// bzero(client.request, sizeof(char) * BASE_REQUEST_SIZE);
		client.capacity = BASE_REQUEST_SIZE;
		client.received = 0;
		return true;
	}
	else
	{
		std::cout << "closing the connection " << std::endl;
		return false;
	}
	return false;

}

// returns true if clients dropped
bool			Server::receive_request(std::vector<ClientInfo>::iterator &it, char **env)
{
	if (it->received == it->capacity)
	{
		// it = this->send_400(*it);
		// return true;
		it->capacity *= 2;
		char *temp = (char *) malloc(sizeof(char) * it->capacity);
		memcpy(temp, it->request, it->received);
		free(it->request);
		it->request = temp;
    }
	int r = recv(it->socket,
		it->request + it->received,
		it->capacity - it->received, 0);
	if (r < 1)
	{
		std::cout << "Unexpected disconnect from " << this->get_client_address(*it) << std::endl;
		std::cerr << strerror(errno) << std::endl;
		it = this->drop_client(*it);
		return true ;
	}
	else
	{
		it->last_received = time(NULL);
		it->received += r;
		std::cout <<  it->received << " bytes received from client: " << it->socket  << std::endl;
		if (Request::request_is_complete(it->request, it->received)) // true if request is fully received; start processing
		{
			std::cout <<  it->received << " total bytes received from client: " << it->socket  << std::endl;
			Request request(it->request, it->received);
			request._env = env;
			if (!this->serve_resource(*it, request))
			{
			    it = this->drop_client(*it);
			    return true ;
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
		std::cerr << "getaddrinfo() failed. (" << errno << ")" << gai_strerror(errno) << std::endl;
		return 1;
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
