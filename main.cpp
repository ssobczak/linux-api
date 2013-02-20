/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "args.h"
#include "config.h"

using namespace std;

int main(int argc, char* argv[]) {
	Args args(argc, argv);

	printf("Will load config from %s.\n", args.config.c_str());
	Config cfg;
	cfg.port = "9876";

	struct addrinfo *server_info;
	{
		// initialize all members
		struct addrinfo hints = {};

		// use IPv4 or IPv6, whichever
		hints.ai_family = AF_UNSPEC;

		// TODO make it the same as in socket()
		hints.ai_socktype = SOCK_STREAM;
		// fill in my IP for me
		hints.ai_flags = AI_PASSIVE;

    	int status = getaddrinfo(NULL, cfg.port.c_str(), &hints, &server_info);
    	if (status != 0) {
    		fprintf(stderr, "Error opening socket: %s\n", gai_strerror(status));
    		exit(1);
    	}
    }

	// protocol family: internet, connection-style socket
	int socket_fd = -1;
	for (struct addrinfo* p = server_info; p != NULL; p = p->ai_next) {
		socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (socket_fd == -1) {
			perror("Error opening socket");
			continue;
		}

		int yes = 1;
		// allow for socket reuse - prevent the "Address already in use" error message
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
			perror("error setting socket options");
		    exit(1);
		}

		if (bind(socket_fd, p->ai_addr, p->ai_addrlen) != 0) {
			perror("Error binding socket.");

			if (close(socket_fd) != 0) {
				perror("Error closing socket");
				exit(1);
			}
			continue;
		}
	}

	freeaddrinfo(server_info);

	// failed to bind
	if (socket_fd == -1) {
		fprintf(stderr, "Failed to bind socket.\n");
		exit(1);
	}



	if (close(socket_fd) != 0) {
		perror("Error closing socket.");
		exit(1);
	}


	return EXIT_SUCCESS;
}
