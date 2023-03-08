
#include "Server.hpp"

Server::Server(ServerConfig const &config)
	: _config(config)
{
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
	this->_config = server._config;
	this->_socket = server._socket;
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
	close(client.socket);
	std::vector<ClientInfo>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (&client == it.base())
		{
			return (_clients.erase(it));
		}
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
fd_set  Server::wait_on_clients(std::set<int> const &sockets,  std::vector<Server>  &servers)
{
	fd_set	reads;// a struct which will hold all our active sockets
	FD_ZERO(&reads);// Initialize fd_set reads to have zero bits for all file descriptors.
	int max_socket = -1; // this var will always have the largest socket fd

	struct timeval timeout;
	timeout.tv_sec = 10; // 10 second timeout
	timeout.tv_usec = 0;

	// first we add sockets used for listening
	std::set<int>::iterator s = sockets.begin();
	while (s != sockets.end())
	{
		FD_SET(*s, &reads);
		if (*s > max_socket)
			max_socket = *s;
		++s;
	}
	std::vector<Server>::iterator serv = servers.begin();
	while (serv != servers.end())
	{
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

	// select indicates which of the specified file descriptors is ready for reading, ready for writing, or has an error condition pending
	if (select(max_socket + 1, &reads, 0, 0, &timeout) < 0) {
		// fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
		// exit(1);
		// throw ? exceptions to-do
	}
	return reads;
}

std::set<int>	Server::create_sockets(std::vector<Server> &servers)
{
	std::set<int> sockets;
	bool		is_set ;
	int i = 0;

	while (i < servers.size())
	{
		is_set = false;
		// we check if there are there is a server with the same port
 		int j = 0;
		while (j < i)
		{
			if (servers[j].get_config()._port == servers[i].get_config()._port)
			{
				servers[i].set_socket(servers[j].get_socket());
				is_set = true;
			}
			j++;
		}
		if (!is_set)
		{
			servers[i].create_socket(servers[i].get_config()._host.c_str(),
				servers[i].get_config()._port.c_str());
			sockets.insert(servers[i].get_socket());
		}
		i++;
	}
	return sockets;
}

// fd_set		Server::wait_on_clients(int server)
// {
// 	fd_set reads;		// a struct which will hold all our active sockets
// 	FD_ZERO(&reads);	// Initialize fd_set reads to have zero bits for all file descriptors.
// 	FD_SET(server, &reads);	// set the bit for the server fd in reads
// 	int max_socket = server; // this var will always have the largest socket fd

// 	std::vector<ClientInfo>::iterator it = _clients.begin();
// 	while (it != _clients.end())
// 	{
// 		FD_SET(it->socket, &reads);
// 		if (it->socket > max_socket)
// 			max_socket = it->socket;
// 		++it;
// 	}
// 	// select indicates which of the specified file descriptors is ready for reading, ready for writing, or has an error condition pending
// 	if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
// 		// fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
// 		// exit(1);
// 		// throw ? exceptions to-do
// 	}
// 	return reads;
// }

// can be one or multiple
void	Server::ack_client(std::vector<Server> &servers, int socket, ClientInfo &client)
{
	int i = 0;
	while (i < servers.size())
	{
		if (servers[i].get_socket() == socket)
		{
			
			servers[i].insert_client(client);
			if (client.socket < 0) {
				fprintf(stderr, "accept() failed. (%d)\n",
					GETSOCKETERRNO());
				exit(1);
			}
			std::cout << "###############################" << std::endl;
			std::cout << "New connection from " << servers[i].get_client_address(client)
				<< " : " << servers[i].get_config()._port << " using socket " << client.socket << std::endl;

			return ;
		}
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

const std::string get_content_type(const char* path) {
	const char *last_dot = strrchr(path, '.');
	if (last_dot) {
		if (strcmp(last_dot, ".css") == 0) return "text/css";
		if (strcmp(last_dot, ".csv") == 0) return "text/csv";
		if (strcmp(last_dot, ".gif") == 0) return "image/gif";
		if (strcmp(last_dot, ".htm") == 0) return "text/html";
		if (strcmp(last_dot, ".html") == 0) return "text/html";
		if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
		if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
		if (strcmp(last_dot, ".jpg") == 0) return "image/jpeg";
		if (strcmp(last_dot, ".js") == 0) return "application/javascript";
		if (strcmp(last_dot, ".json") == 0) return "application/json";
		if (strcmp(last_dot, ".png") == 0) return "image/png";
		if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
		if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
		if (strcmp(last_dot, ".txt") == 0) return "text/plain";
	}
	return "application/octet-stream";
}

// returns whether the connection should be open or not
bool		Server::serve_resource(ClientInfo &client, Request &request, std::vector<ServerConfig> &configs)
{
	std::cout << "server_resource " << get_client_address(client) << " " << request._path << std::endl;
	// if (request._path == "/")
		// request._path = "/index.html";

	// if (request._method == "OPTIONS" && request._header["access-control-request-method"] != "" && request._header["access-control-request-headers"] != "")
	// {
	// 	// preflight request ?
	// 	const char *response_headers =
    //     "HTTP/1.1 204 No Content\r\n"
    //     "Access-Control-Allow-Origin: *\r\n"
    //     "Access-Control-Allow-Methods: GET, POST\r\n"
    //     "Access-Control-Allow-Headers: Content-Type\r\n"
    //     "Access-Control-Max-Age: 86400\r\n"
    //     "\r\n";
    //   send(client.socket, response_headers, strlen(response_headers), 0);
	//   return false;
	// }
	
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

	std::string response = get_response(request, configs);
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
	// std::cout << "Configuring local address......" << std::endl;

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	struct addrinfo *bind_address;
	getaddrinfo(host, port, &hints, &bind_address);

	std::cout << "Creating socket... " << host << " : " << port << std::endl;
	int socket_listen;
	socket_listen = socket(bind_address->ai_family,
	bind_address->ai_socktype, bind_address->ai_protocol);
	if (socket_listen < 0)
	{
		fprintf(stderr, "socket() failed. (%d)\n", errno);
		exit(1);
	}
	std::cout << "Binding socket to local address..." << std::endl;
	if (bind(socket_listen,
		bind_address->ai_addr, bind_address->ai_addrlen))
	{
		fprintf(stderr, "bind() failed. (%d)\n", errno);
		exit(1);
	}
	freeaddrinfo(bind_address);
	std::cout << "Listening to port " << port << std::endl;	
	if (listen(socket_listen, SOMAXCONN) < 0)
	{
		fprintf(stderr, "listen() to %s failed. (%d)\n", port, errno);
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

std::vector<ClientInfo> &Server::get_clients()
{
	return _clients;
}

void	Server::insert_client(ClientInfo &client)
{
	_clients.insert(_clients.begin(), client);
}


ServerConfig	& Server::get_config()
{
	return _config;
}

int				Server::get_socket()
{
	return _socket;
}

void			Server::set_socket(int socket)
{
	_socket = socket;
}

bool			Server::receive_request(std::vector<ClientInfo>::iterator &it, Config &config)
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
		it->request[it->received] = 0;
		if (Request::request_is_complete(it->request, it->received)) // true if request is fully received; start processing
		{
			Request request(it->request);
			if (!this->serve_resource(*it, request, config.get_configs()))
			{
			    it = this->drop_client(*it);
			    return true ;
			}
		}
	}
	return false ;
}