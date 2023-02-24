#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define ISVALIDSOCKET(s) ((s) >= 0)


#include <stdio.h>
#include <string.h>
#include <time.h>

int	main()
{
	printf("Configuring local address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));	//zeroing

	hints.ai_family = AF_INET;	// protocol either AF_INET (ipv4), or AF_INET6 (ipv6)
	hints.ai_socktype = SOCK_STREAM;	// SOCK_STREAM means we are going to use TCP, change to SOCK_DGRAM for UDP
	hints.ai_flags = AI_PASSIVE;
	/*
		AI_PASSIVE
		This is telling getaddrinfo() that we want it to bind to the wildcard address. That is, we are asking getaddrinfo() to set up the address,
		so we listen on any available network interface.
	*/ 

	struct addrinfo *bind_address;
	// getaddresinfo() fills addrinfo with needed information
	//	it also takes hint parameters
	//  second parameter is the port we gonna listen for connections on
	getaddrinfo(0, "8080", &hints, &bind_address);  // this will generate and address thats suitable for bind().

	printf("Creating socket...\n");
	int socket_listen;
	socket_listen = socket(bind_address->ai_family,			//creating socket file descriptor
	bind_address->ai_socktype, bind_address->ai_protocol);

	if (!ISVALIDSOCKET(socket_listen)) {
		fprintf(stderr, "socket() failed. (%d)\n", errno);
		return 1;
	}

	//dual sockets ?
	// int option = 0;
	// if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY,
	// 	(void*)&option, sizeof(option))) {
	// 		fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
	// 		return 1;
	// }
	
	printf("Binding socket to local address...\n");
	// after creating the socket, we need to call bind to associate it with our address we got from getaddrinfo()
	if (bind(socket_listen,bind_address->ai_addr, bind_address->ai_addrlen)) {
		fprintf(stderr, "bind() failed. (%d)\n", errno);
		return 1;
	}
	freeaddrinfo(bind_address);	// we no longer bind_address, we free it

	printf("Listening...\n");
	/* once the socket has been created and bound to a local address, 
	 we can start listening for connections with the listen 
	 the second parameter refers to how man connectons it is allowed to queue up. 
	 the operating system will reject new connections if the 10 connections are queued up */
	if (listen(socket_listen, 10) < 0) {
		fprintf(stderr, "listen() failed. (%d)\n", errno);
		return 1;
	}

	fd_set master; //  a struct to store all of the active sockets.
	FD_ZERO(&master);	// clears all file descriptors from the set
	FD_SET(socket_listen, &master); // add socket_listen to master set
	int max_socket = socket_listen;	// we define max_socket where we store the highest numbered fd

	printf("Waiting for connections...\n");
	while(1) {

		fd_set reads;
		reads = master;	// select will change our set so we use a copy
		// we give select a set of sockets, and it  tells us which are ready to be
		if (select(max_socket + 1, &reads, 0, 0, 0) < 0) {
			fprintf(stderr, "select() failed. (%d)\n", errno);
			return 1;
		}
		int i = 1;
		while (i <= max_socket) {
			// printf("LULE\n");
			if (FD_ISSET(i, &reads)) {	// FD_ISSET() is only true for sockets that are ready to be read
				// handle socket
				if (i == socket_listen) {
					struct sockaddr_storage	client_address;
					socklen_t	client_len = sizeof(client_address);
					int socket_client = accept(socket_listen,
						(struct sockaddr*) &client_address,
						&client_len);
					if (!ISVALIDSOCKET(socket_client)) {
						fprintf(stderr, "accept() failed. (%d)\n", errno);
						return (1);
					}

					FD_SET(socket_client, &master);
					if (socket_client > max_socket)
						max_socket = socket_client;
					
					char address_buffer[100];
					getnameinfo((struct sockaddr *) &client_address,
						client_len,
						address_buffer,
						sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
					printf("New connection from %s\n", address_buffer);
				} else {
					char request[1024];
					int bytes_received = recv(i, request, 1024, 0);
					if (bytes_received < 1) {
						FD_CLR(i, &master); // we clear the fd from the master 
						close(i);
						continue ;
					}
					printf("Received %d bytes.\n", bytes_received);
					printf("Sending response...\n");
					const char *response =
					"HTTP/1.1 200 OK\r\n"
					"Connection: close\r\n"
					"Content-Type: text/plain\r\n\r\n"
					"Local time is: ";
					int bytes_sent = send(i, response, strlen(response), 0);
					printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

					time_t timer;
					time(&timer);
					char *time_msg = ctime(&timer);
					 bytes_sent = send(i, time_msg, strlen(time_msg), 0);
					printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));
					FD_CLR(i, &master); // add this to code
					close(i);
				}
			}
			i++;
		}
	}
	printf("Closing listening socket...\n");
	close(socket_listen);
	printf("Finished.\n");

	
	return 0;
}



// struct sockaddr_storage client_address;
	// socklen_t client_len = sizeof(client_address);
	// int socket_client = accept(socket_listen,
	// (struct sockaddr*) &client_address, &client_len);
	// if (!ISVALIDSOCKET(socket_client)) {
	// 	fprintf(stderr, "accept() failed. (%d)\n", errno);
	// 	return 1;
	// }

	// printf("Client is connected... ");
	// char address_buffer[100];
	// getnameinfo((struct sockaddr*)&client_address,
	// client_len, address_buffer, sizeof(address_buffer), 0, 0,
	// NI_NUMERICHOST);
	// printf("%s\n", address_buffer);

	// printf("Reading request...\n");
	// char request[1024];
	// int bytes_received = recv(socket_client, request, 1024, 0);
	// printf("Received %d bytes.\n", bytes_received);

	// printf("Sending response...\n");
	// const char *response =
	// "HTTP/1.1 200 OK\r\n"
	// "Connection: close\r\n"
	// "Content-Type: text/plain\r\n\r\n"
	// "Local time is: ";
	// int bytes_sent = send(socket_client, response, strlen(response), 0);
	// printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

	// time_t timer;
	// time(&timer);
	// char *time_msg = ctime(&timer);
	// bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
	// printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

	// printf("Closing connection...\n");
 	// close(socket_client);