/*
 * Show's an IP address for a given host on the command line
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf(stderr, "usage: showip hostname\n");
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

	printf("IP addresss for %s:\n\n", argv[1]);

	for(p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;

		// get the point to the address itself
		// different fields in ipv4 and ipv6
		if (p->ai_family == AF_INET) {
			// IPV4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPV4";
		} else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPV6";
		}

		// convert the ip to a string and print it
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf("    %s: %s\n", ipver, ipstr);
	}

	freeaddrinfo(res);

	// everything went well tell the os as well
	return 0;
}
