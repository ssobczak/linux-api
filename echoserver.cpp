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

void EchoServer::register_event(const SocketEvents& event, const int& socket) {
	char buff[50];
	int len;

	switch (event) {
		case NewClient:
			std::cout << "Got a new client: " << socket << std::endl;
			break;
		case DataArrived:
			len = recv(socket, buff, 50, MSG_DONTWAIT);
			if (len == -1) {
				if (errno != EAGAIN && errno != EWOULDBLOCK) {
					perror("failed to read from client socket");
				}
			} else {
				std::cout << socket << ": " << std::string(buff, len) << std::endl;
			}
			break;
		default:
			break;
	}
}
