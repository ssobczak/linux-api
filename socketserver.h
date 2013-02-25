/*
 *  Created on: Feb 21, 2013
 *      Author: ssobczak
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <set>
#include <string>

#include "config.h"
#include "eventqueue.h"
#include "listenner.h"

/**
 * thread-safe
 */
class SocketsServer {
public:
	SocketsServer();
	virtual ~SocketsServer();

	bool set_config(const Config& cfg);
	bool run();
	bool stop();

	// for running on a thread
	static void* run_server(void* server);

protected:
	virtual void on_new_client(int socket) = 0;
	virtual void on_sock_ready(int socket) = 0;

	void close_connection(int socket);

// non-public functions are not thread-safe
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

	EventQueue close_clients_;

	void init();

	bool is_started();
	bool start_if_possible();
	bool cleanup();

	bool init_server_socket();
	void do_work();

	bool accept_client();
	bool remove_client(int client);

	bool close_connections();
};

#endif /* SERVER_H_ */
