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

/*			socket()
	The socket() function takes three arguments:
	peer_address->ai_family specifies the address family used for the socket. This is typically AF_INET for IPv4 or 	AF_INET6 for IPv6.
	peer_address->ai_socktype specifies the type of socket to be created, such as SOCK_STREAM for a stream socket or 	SOCK_DGRAM for a datagram socket.
	peer_address->ai_protocol specifies the protocol to be used with the socket. This is typically 0 for the default 	protocol for the given ai_family and ai_socktype combination.
*/


/*			getaddrinfo()
	getaddrinfo() is very flexible about how it takes inputs. The hostname could be a
	domain name like example.com or an IP address such as 192.168.17.23 or ::1. The port
	can be a number, such as 80, or a protocol, such as http.
*/


/*			connect()
	connect() takes three arguments—the socket, the remote address, and the remote address
	length. It returns 0 on success, so we print an error message if it returns non-zero. This call
	to connect() is extremely similar to how we called bind() in Chapter 2, Getting to Grips
	with Socket APIs. Where bind() associates a socket with a local address, connect()
	associates a socket with a remote address and initiates the TCP connection.
*/

int main( int argc, char **argv ) {

	if (argc < 3) {
		fprintf(stderr, "usage: tcp_client hostname port\n");
		return (1);
	}

	printf("Configuring remote address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	struct addrinfo *peer_address;
	if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
		fprintf(stderr, "getaddrinfo() failed. (%d)\n", errno);
		return (1);
	}

	printf("Remote address is: ");
	char address_buffer[100];
	char service_buffer[100];
	getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
		address_buffer, sizeof(address_buffer),
		service_buffer, sizeof(service_buffer),
		NI_NUMERICHOST);
	printf("%s %s\n", address_buffer, service_buffer);


	printf("Creating socket...\n");
	int socket_peer;
	socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
	if (!ISVALIDSOCKET(socket_peer)) {
		fprintf(stderr, "socket() failed. (%d)\n", errno);
		return (1);
	}

	printf("Connecting...\n");
	if (connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)) {
		fprintf(stderr, "connect() failed. (%d)\n", errno);
		return (1);
	}
	freeaddrinfo(peer_address);

	printf("Connected.\n");
	printf("To send data, enter text followed by enter.\n");

	while (1) {
		fd_set	reads;
		FD_ZERO(&reads);
		FD_SET(socket_peer, &reads);
		FD_SET(0, &reads);

		// struct timeval timeout;
		// timeout.tv_sec = 0;
		// timeout.tv_usec = 100000; // no need for timeout

		if ( select(socket_peer + 1, &reads, 0, 0, 0) < 0) {
			fprintf(stderr, "select() failed. (%d)\n", errno);
			return (1);
		}
		if (FD_ISSET(socket_peer, &reads)) {
			char read[4096];
			int bytes_received = recv(socket_peer, read, 4096, 0);
			if (bytes_received < 1) {
				printf("Connection closed by peer.\n");
				break ;
			}
			printf("Received (%d bytes): %.*s",
				bytes_received, bytes_received, read);
		}
		if (FD_ISSET(0, &reads)) {
			char read[4096];
			if (!fgets(read, 4096, stdin))
				break ;
			int bytes_sent = send(socket_peer, read, strlen(read), 0);
			printf("Sent %d bytes.\n", bytes_sent);
		}
	}
	printf("Closing socket...\n");
 	close(socket_peer);
	printf("Finished\n");

	return (0);
}