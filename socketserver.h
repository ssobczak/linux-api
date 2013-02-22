/*
 *  Created on: Feb 21, 2013
 *      Author: ssobczak
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <set>
#include <string>

#include "listenner.h"

class Config;

enum SocketEvents {
	NewClient = 0,
};

class SocketsServer : public Notifier<SocketEvents, int> {
public:
	SocketsServer();
	virtual ~SocketsServer();

	bool start(const Config &cfg);
	bool stop();

	// accept a single connection
	int accept_client();
	bool remove_client(int client);

	bool broadcast(const std::string& msg) const;

private:
	int server_socket;

	typedef std::set<int> ClientsMap;
	ClientsMap clients;
};

#endif /* SERVER_H_ */
