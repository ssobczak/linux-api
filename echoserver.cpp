/*
 *  Created on: Feb 25, 2013
 *      Author: ssobczak
 */

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

#include "echoserver.h"

void EchoServer::on_new_client(int socket) {
	std::cout << "Got a new client: " << socket << std::endl;
}

void EchoServer::on_sock_ready(int socket) {
	char buff[50];
	int len;
	len = recv(socket, buff, 50, MSG_DONTWAIT);
	if (len == -1) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			perror("failed to read from client socket");
		}
	} else if (len == 0) {
		close_connection(socket);
	} else {
		std::cout << socket << ": " << std::string(buff, len) << std::flush;
	}
}
