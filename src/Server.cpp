
#include "Server.hpp"

Server::Server()
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

void		Server::drop_client(ClientInfo & client)
{
	close(client.socket);

	std::vector<ClientInfo>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (&client == it.base())
		{
			_clients.erase(it);
			return ;
		}
		++it;
	}
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

// int represents the socket here
fd_set		Server::wait_on_clients(int server)
{
	fd_set reads;		// a struct which will hold all our active sockets
	FD_ZERO(&reads);	// Initialize fd_set reads to have zero bits for all file descriptors.
	FD_SET(server, &reads);	// set the bit for the server fd in reads
	int max_socket = server; // this var will always have the largest socket fd

	std::vector<ClientInfo>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		FD_SET(it->socket, &reads);
		if (it->socket > max_socket)
			max_socket = it->socket;
		++it;
	}
	// select indicates which of the specified file descriptors is ready for reading, ready for writing, or has an error condition pending
	if (select(max_socket+1, &reads, 0, 0, 0) < 0) {
		// fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
		// exit(1);
		// throw ? exceptions to-do
	}
	return reads;
}

void		Server::send_400(ClientInfo &client)
{
	const char *c400 = "HTTP/1.1 400 Bad Request\r\n"
		"Connection: close\r\n"
		"Content-Length: 11\r\n\r\nBad Request";

	send(client.socket, c400, strlen(c400), 0);
	drop_client(client);
}

void		Server::send_404(ClientInfo &client)
{
	const char *c404 = "HTTP/1.1 404 Not Found\r\n"
		"Connection: close\r\n"
		"Content-Length: 9\r\n\r\nNot Found";

	send(client.socket, c404, strlen(c404), 0);
	drop_client(client);
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


void		Server::serve_resource(ClientInfo &client, Request &request)
{
	std::cout << "server_resource " << get_client_address(client) << " " << request._path << std::endl;\
	if (request._path == "/")
		request._path = "/index.html";
	if (request._path.size() > 100)
	{
		send_400(client);
		return ;
	}
	if (request._path.find("..") != std::string::npos)
	{
		send_404(client);
		return ;
	}
	char full_path[128];
 	sprintf(full_path, "public%s", request._path.c_str());

	// FILE *fp = fopen(full_path, "rb");
 	// if (!fp) {
		// send_404(client);
		// return;
 	// }
	// fseek(fp, 0L, SEEK_END);
	// size_t cl = ftell(fp);
	// rewind(fp);
	request._path = full_path;
	std::cout<<request._body<<std::endl;
	std::string response = get_response(request);
	send(client.socket, response.c_str(), response.size(), 0);
	// std::string ct = get_content_type(full_path);
// 
 	// char buffer[BSIZE];
	// sprintf(buffer, "HTTP/1.1 200 OK\r\n");
	// send(client.socket, buffer, strlen(buffer), 0);
	// sprintf(buffer, "Connection: close\r\n");
	// send(client.socket, buffer, strlen(buffer), 0);
	// sprintf(buffer, "Conhtent-Lengt: %lu\r\n", cl);
	// send(client.socket, buffer, strlen(buffer), 0);
	// sprintf(buffer, "Content-Type: %s\r\n", ct.c_str());
	// send(client.socket, buffer, strlen(buffer), 0);
	// sprintf(buffer, "\r\n");
	// send(client.socket, buffer, strlen(buffer), 0);

	// int r = fread(buffer, 1, BSIZE, fp);
	// while (r) {
		// send(client.socket, buffer, r, 0);
		// r = fread(buffer, 1, BSIZE, fp);
	// }

	// fclose(fp);
 	drop_client(client);

	/*
	Note that send() may block on large files. In a truly robust, production-ready server, you
	would need to handle this case. It could be done by using select() to determine when
	each socket is ready to read. Another common method is to use fork() or similar APIs to
	create separate threads/processes for each connected client.
	*/

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
	getaddrinfo(host, port, &hints, &bind_address);

	std::cout << "Creating socket..." << std::endl;
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

	std::cout << "Listening..." << std::endl;	
	if (listen(socket_listen, 10) < 0)
	{
		fprintf(stderr, "listen() failed. (%d)\n", errno);
		exit(1);
	}
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

void	Server::add_location(Location &location)
{
	_locations.push_back(location);
}
