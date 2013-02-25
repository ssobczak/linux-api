/*
 *  Created on: Feb 25, 2013
 *      Author: ssobczak
 */

#ifndef ECHOSERVER_H_
#define ECHOSERVER_H_

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "listenner.h"
#include "socketserver.h"

class EchoServer : public SocketsServer {
public:
	EchoServer() {}
	virtual ~EchoServer() {}

protected:
	virtual void on_new_client(int socket) {
		std::cout << "Got a new client: " << socket << std::endl;
	}

	virtual void on_sock_ready(int socket) {
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
};

#endif /* ECHOSERVER_H_ */
