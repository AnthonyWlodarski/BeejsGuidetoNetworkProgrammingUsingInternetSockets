/*
 * gets a socket descriptor for a given address
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
	struct addrinfo hints, *res, *p;
	int status, s;
	char ipstr[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf(stderr, "usage: socket hostname\n");
		return 1;
	}

	// set the memory to 0 for all fields
	memset(&hints, 0, sizeof hints);
	// we don't know if ipv4 or ipv6
	hints.ai_family = AF_UNSPEC;
	// we are using a socket stream not socket datagram
	hints.ai_socktype = SOCK_STREAM;

	// if getaddrinfo returns abnormal information we will print out the error
	// to stderr
	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
	}

	if((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		int socket_error = errno;
		// if we can't get the right descriptor print out a error message and save errno
		perror("Unable to get socket descriptor");
		fprintf(stderr, "Unable to get socket descriptor: %s\n", strerror(socket_error));
		return 2;
	}


	freeaddrinfo(res);
	// everything went well tell the os as well
	return 0;
}
