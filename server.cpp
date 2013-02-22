/*
 *  Created on: Feb 21, 2013
 *      Author: ssobczak
 */

#include "server.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "config.h"

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

Server::Server() : server_socket(-1) {
}

Server::~Server() {
	stop();
}

bool Server::start(const Config& cfg) {
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

    	int status = getaddrinfo(NULL, cfg.port.c_str(), &hints, &server_info);
    	if (status != 0) {
    		fprintf(stderr, "Error opening socket: %s\n", gai_strerror(status));
    		return false;
    	}
    }

	for (struct addrinfo* p = server_info; p != NULL; p = p->ai_next) {
		server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (server_socket == -1) {
			perror("Error opening socket");
			continue;
		}

		int yes = 1;
		// allow for socket reuse - prevent the "Address already in use" error message
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) != 0) {
			perror("error setting socket options");
			return false;
		}

		if (bind(server_socket, p->ai_addr, p->ai_addrlen) != 0) {
			perror("Error binding socket.");

			if (close(server_socket) != 0) {
				perror("Error closing socket");
				return false;
			}
			continue;
		}
	}

	freeaddrinfo(server_info);

	if (server_socket == -1) {
		fprintf(stderr, "Failed to bind socket.\n");
		return false;
	}

    if (listen(server_socket, cfg.backlog) != 0) {
        perror("Failed to listen on a socket");
        return false;
    }

	return true;
}

bool Server::stop() {
	for (ClientsMap::iterator it = clients.begin(); it != clients.end();) {
		remove_client(*it++);
	}

	return clients.empty() && close_socket(server_socket);
}

int Server::accept_client() {
	struct sockaddr addr;
	size_t addr_len = sizeof(addr);

	int client_sock_fd = accept(server_socket, &addr, &addr_len);
	if (client_sock_fd == -1) {
		perror("Failed to accept connection");
	}

	clients.insert(client_sock_fd);
	notify(NewClient, client_sock_fd);
	return client_sock_fd;
}

bool Server::remove_client(int client) {
	ClientsMap::iterator it = clients.find(client);
	if (it == clients.end()) {
		return false;
	}

	if (!close_socket(*it)) {
		return false;
	}

	clients.erase(client);
	return true;
}

bool Server::broadcast(const string& msg) const {
	bool success = true;

	for (ClientsMap::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		if (!send_message(msg, *it)) {
			success = false;
		}
	}

	return success;
}

