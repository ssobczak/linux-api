/*
 *  Created on: Feb 21, 2013
 *      Author: ssobczak
 */

#include "socketserver.h"

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>

#include "config.h"
#include "listenner.h"

using std::string;

bool send_message(const string& msg, int client) {
	size_t sent = 0;

	do {
		int status = send(client, msg.c_str() + sent, msg.size() - sent, 0);

		if (status == -1) {
			perror("Failed to send a message");
			return false;
		}

		sent += status;
	} while (sent != msg.size());

	return true;
}

bool close_socket(int socket) {
	if (socket != -1 && close(socket) != 0) {
		perror("Error closing socket.");
		return false;
	}

	socket = -1;
	return true;
}

SocketsServer::SocketsServer() {
	pthread_mutex_init(&state_mutex_, NULL);
	config_ = Config();
	server_state = StateInvalid;
	server_socket_ = -1;
}

SocketsServer::~SocketsServer() {
	stop();
	pthread_mutex_destroy(&state_mutex_);
}

bool SocketsServer::set_config(const Config& cfg) {
	bool success = false;

	pthread_mutex_lock (&state_mutex_);
	if (server_state == StateInvalid || server_state == StateStopped) {
		config_ = cfg;
		server_state = StateStopped;
		success = true;
	}
	pthread_mutex_unlock(&state_mutex_);

	return success;
}

bool SocketsServer::is_started() {
	pthread_mutex_lock (&state_mutex_);
	bool ret = (server_state == StateStarted);
	pthread_mutex_unlock(&state_mutex_);
	return ret;
}

bool SocketsServer::start_if_possible() {
	bool started = false;

	pthread_mutex_lock (&state_mutex_);
	if (server_state == StateStopped) {
		server_state = StateStarted;
		started = true;
	}
	pthread_mutex_unlock(&state_mutex_);

	return started;
}

bool SocketsServer::init_server_socket() {
	struct addrinfo *server_info;
		{
			// initialize all members
			struct addrinfo hints = {};

			// use IPv4 or IPv6, whichever
			hints.ai_family = AF_UNSPEC;

			// connection-style socket
			hints.ai_socktype = SOCK_STREAM;

			// fill in my IP for me
			hints.ai_flags = AI_PASSIVE;

	    	int status = getaddrinfo(NULL, config_.port.c_str(), &hints, &server_info);
	    	if (status != 0) {
	    		fprintf(stderr, "Error opening socket: %s\n", gai_strerror(status));
	    		return false;
	    	}
	    }

		for (struct addrinfo* p = server_info; p != NULL; p = p->ai_next) {
			server_socket_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
			if (server_socket_ == -1) {
				perror("Error opening socket");
				continue;
			}

			int yes = 1;
			// allow for socket reuse - prevent the "Address already in use" error message
			if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
				perror("error setting socket options");
				return false;
			}

			if (bind(server_socket_, p->ai_addr, p->ai_addrlen) != 0) {
				perror("Error binding socket.");

				if (close(server_socket_) != 0) {
					perror("Error closing socket");
					return false;
				}
				continue;
			}
		}

		freeaddrinfo(server_info);

		if (server_socket_ == -1) {
			fprintf(stderr, "Failed to bind socket.\n");
			return false;
		}

	    if (listen(server_socket_, config_.backlog) != 0) {
	        perror("Failed to listen on a socket");
	        return false;
	    }

	    printf("Listenning on port %s\n", config_.port.c_str());
		return true;
}

bool SocketsServer::run() {
	if (!start_if_possible()) {
		return false;
	}

	if (!init_server_socket()) {
		stop();
		return false;
	}

	while(is_started()) {
		close_connections();
		do_work();
	}

	return cleanup();
}

bool SocketsServer::stop() {
	bool stopped = false;

	pthread_mutex_lock (&state_mutex_);
	if (server_state == StateStarted) {
		server_state = StateStopped;
		stopped = true;
	}
	pthread_mutex_unlock(&state_mutex_);

	return stopped;
}

bool SocketsServer::cleanup() {
	bool success = true;

	for (ClientsSet::iterator it = clients_.begin(); it != clients_.end();) {
		if (!remove_client(*it++)) {
			success = false;
		}
	}

	if (!clients_.empty() || close_socket(server_socket_)) {
		success = false;
	}

	return success;
}

bool SocketsServer::accept_client() {
	struct sockaddr addr;
	size_t addr_len = sizeof(addr);

	int client_sock_fd = accept(server_socket_, &addr, &addr_len);
	if (client_sock_fd == -1) {
		perror("Failed to accept connection");
		return false;
	}

	if (fcntl(client_sock_fd, F_SETFL, O_NONBLOCK) == -1) {
		perror("Failed to set client socket to nonblocking mode");
		return false;
	}

	clients_.insert(client_sock_fd);
	on_new_client(client_sock_fd);

	return true;
}

bool SocketsServer::remove_client(int client) {
	ClientsSet::iterator it = clients_.find(client);
	if (it == clients_.end()) {
		return false;
	}

	if (!close_socket(*it)) {
		return false;
	}

	clients_.erase(client);
	std::cout << "Closed " << client << std::endl;
	return true;
}

void SocketsServer::do_work() {
	fd_set sockets_set;
	FD_ZERO(&sockets_set);

	// TODO maintain a copy instead
	FD_SET(server_socket_, &sockets_set);
	for (ClientsSet::const_iterator it = clients_.begin(); it != clients_.end(); it++) {
		FD_SET(*it, &sockets_set);
	}

	int max_sock = server_socket_;
	if (!clients_.empty()) {
		max_sock = std::max(max_sock, *clients_.rbegin());
	}

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100;

	int status = select(max_sock + 1, &sockets_set, NULL, NULL, &timeout);
	if (status < 0) {
		if (errno != EINTR) {
			perror("Failed to select socket");
		}
	}

	if (FD_ISSET(server_socket_, &sockets_set)) {
		accept_client();
	}

	for (ClientsSet::const_iterator it = clients_.begin(); it != clients_.end(); it++) {
		if (FD_ISSET(*it, &sockets_set)) {
			on_sock_ready(*it);
		}
	}
}

void* SocketsServer::run_server(void* server) {
	SocketsServer* srv = static_cast<SocketsServer*>(server);

	if (srv == NULL) {
		fprintf(stderr, "Failed to convert param to SocketServer");
		return NULL; // false
	}

	srv->run();
	return NULL; // result
}

void SocketsServer::close_connection(int socket) {
	close_clients_.add(EventQueue::callback([this, socket]() {
		return this->remove_client(socket);
	}));
}

bool SocketsServer::close_connections() {
	return close_clients_.flush();
}
