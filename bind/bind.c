/*
 * gets a socket descriptor for the local host and binds to a port
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	struct addrinfo hints, *res;
	int status, sockfd;

	if (argc != 2) {
		fprintf(stderr, "usage: bind port\n");
		return 1;
	}

	// set the memory to 0 for all fields
	memset(&hints, 0, sizeof hints);
	// we don't know if ipv4 or ipv6
	hints.ai_family = AF_UNSPEC;
	// we are using a socket stream not socket datagram
	hints.ai_socktype = SOCK_STREAM;
	// fill in my ip for me
	hints.ai_flags = AI_PASSIVE;

	// if getaddrinfo returns abnormal information we will print out the error
	// to stderr
	if ((status = getaddrinfo(NULL, argv[1], &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
	}

	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		int socket_error = errno;
		// if we can't get the right descriptor print out a error message and save errno
		fprintf(stderr, "Unable to get socket descriptor: %s\n", strerror(socket_error));
		return 2;
	}

	// bind to the port we passed into getaddrinfo();
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		int bind_error = errno;
		// if we can't get the right descriptor print out a error message and save errno
		fprintf(stderr, "Unable to bind on port %s: %s\n", argv[1], strerror(bind_error));
		return 3;
	}

	freeaddrinfo(res);
	// everything went well tell the os as well
	return 0;
}
