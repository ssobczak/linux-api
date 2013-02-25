/*
 *  Created on: Feb 25, 2013
 *      Author: ssobczak
 */

#include "dispatchserver.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

#include "workerspool.h"

DispatchServer::DispatchServer(WorkersPool& wp) : workers_pool_(wp) {
	// TODO Auto-generated constructor stub

}

DispatchServer::~DispatchServer() {
	// TODO Auto-generated destructor stub
}

void DispatchServer::on_new_client(int socket) {
	workers_pool_.add_job(WorkersPool::job(
			std::bind(&DispatchServer::handle_new_client, this, socket)
	));
}

void DispatchServer::on_sock_ready(int socket) {
	workers_pool_.add_job(WorkersPool::job(
			std::bind(&DispatchServer::handle_sock_ready, this, socket)
	));
}

bool DispatchServer::handle_sock_ready(int socket) {
	char buff[50];
	int len;
	len = recv(socket, buff, 50, MSG_DONTWAIT);
	if (len == -1) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			perror("failed to read from client socket");
		}
		return false;
	}

	if (len == 0) {
		close_connection(socket);
	} else {
		std::cout << socket << ": " << std::string(buff, len) << std::flush;
	}
	return true;
}

bool DispatchServer::handle_new_client(int socket) {
	std::cout << "Got a new client: " << socket << std::endl;
	return true;
}
