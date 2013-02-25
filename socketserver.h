/*
 *  Created on: Feb 21, 2013
 *      Author: ssobczak
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <set>
#include <string>

#include "config.h"
#include "listenner.h"

enum SocketEvents {
	NewClient = 0,
	DataArrived = 1,
};

/**
 * thread-safe
 *
 * Examlpe:
 * SocketsServer server;
 * server.init(c);
 * server.add_listenner(l);
 *
 * server.run();
 * server.stop();
 */
class SocketsServer : public Notifier<SocketEvents, int> {
public:
	SocketsServer();
	virtual ~SocketsServer();

	bool set_config(const Config &cfg);
	bool run();
	bool stop();

// non-poublic functions are not thread-safe
private:
	pthread_mutex_t state_mutex_;
	enum {
		StateInvalid = 0,
		StateStopped = 1,
		StateStarted = 2
	} server_state;

	Config config_;

	int server_socket_;
	typedef std::set<int> ClientsSet;
	ClientsSet clients_;

	void init();

	bool is_started();
	bool start_if_possible();
	bool cleanup();

	bool init_server_socket();
	void do_work();

	bool accept_client();
	bool remove_client(int client);
};

#endif /* SERVER_H_ */
