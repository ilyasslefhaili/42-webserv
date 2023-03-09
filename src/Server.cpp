
#include "Server.hpp"

Server::Server(ServerConfig const &config)
{
	_configs.push_back(config);
	_port = config._port;
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
	this->_clients = server._clients;
	this->_configs = server._configs;
	this->_socket = server._socket;
	this->_port = server._port;
	return *this;
}

ClientInfo	Server::get_client(int socket)
{
	std::vector<ClientInfo>::iterator it
		= std::find_if(_clients.begin(), _clients.end(), Server::MatchSocket(socket));
	
	if (it != _clients.end())
		return (*it);
	ClientInfo client;
	client.address_length = sizeof(client.address);
	_clients.insert(_clients.begin(), client);
	return (client);
}

std::vector<ClientInfo>::iterator Server::drop_client(ClientInfo & client)
{
	std::cout << "closing socket: " << client.socket << std::endl;
	close(client.socket);
	std::vector<ClientInfo>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (client.socket == it->socket)
			return (_clients.erase(it));
		++it;
	}
	return (_clients.end());
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
		std::vector<ClientInfo>::iterator cl = serv->get_clients().begin();
		while (cl != serv->get_clients().end())
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
		servers[i].create_socket(servers[i].get_configs().front()._host.c_str(),
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

// returns whether the connection should be open or not
bool		Server::serve_resource(ClientInfo &client, Request &request)
{
	std::cout << "server_resource " << get_client_address(client) << " " << request._path << std::endl;
	if (request._path.size() > 100)
	{
		send_400(client);
		return false;
	}
	if ( request._path == "/favicon.ico" ) // to remove ?
	{
		std::cout << "handle fav icon" << std::endl;
		send_404(client);
		return false;
	}
	if (request._path.find("..") != std::string::npos)
	{
		send_404(client);
		return false;
	}

	std::string response = get_response(request, _configs);
	send(client.socket, response.c_str(), response.size(), 0);
	if (request._header["Connection"] == "keep-alive")
	{
		std::cout << "keeping the connection alive" << std::endl;
		bzero(client.request, sizeof(client.request)); // we clear request in case connection is alive
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


Server::MatchSocket::MatchSocket(int s) : socket(s) {}

bool Server::MatchSocket::operator()(const ClientInfo& obj) const
{
	return obj.socket == socket;
}

Server::MatchPort::MatchPort(std::string p) : port(p) {}

bool Server::MatchPort::operator()(const Server& obj) const
{
	return obj._port == port;
}

std::vector<ClientInfo> &Server::get_clients()
{
	return _clients;
}

void	Server::insert_client(ClientInfo &client)
{
	_clients.push_back(client);
}


std::vector<ServerConfig>	&Server::get_configs()
{
	return _configs;
}

void	Server::add_config(ServerConfig const &config)
{
	_configs.push_back(config);
}


int				Server::get_socket()
{
	return _socket;
}

void			Server::set_socket(int socket)
{
	_socket = socket;
}

// returns true if clients dropped
bool			Server::receive_request(std::vector<ClientInfo>::iterator &it)
{
	if (it->received == MAX_REQUEST_SIZE)
	{
		it = this->send_400(*it);
		return true;
    }
	int r = recv(it->socket,
	it->request + it->received,
	MAX_REQUEST_SIZE - it->received, 0);
	if (r < 1)
	{
		std::cout << "Unexpected disconnect from " << this->get_client_address(*it) << std::endl;
		it = this->drop_client(*it);
		return true ;
	}
	else
	{
		it->last_received = time(NULL);
		it->received += r;
		std::cout << it->received << std::endl;
		if (Request::request_is_complete(it->request, it->received)) // true if request is fully received; start processing
		{
			it->request[it->received] = 0;
			std::cout << strlen(it->request) << std::endl;
			Request request(it->request);
			if (!this->serve_resource(*it, request))
			{
			    it = this->drop_client(*it);
			    return true ;
			}
		}
	}
	return false ;
}

std::string			Server::get_port()
{
	return _port;
}

void				Server::set_port(std::string &port)
{
	_port = port;
}
