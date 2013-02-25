/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#include <pthread.h>
#include <stdlib.h>

#include <sstream>
#include <string>
#include <iostream>

#include "args.h"
#include "config.h"
#include "echoserver.h"
#include "socketserver.h"

int main(int argc, char* argv[]) {
	Args args(argc, argv);

	printf("Will load config from %s.\n", args.config.c_str());
	Config cfg;
	if (!cfg.read_config()) {
		return EXIT_FAILURE;
	}

	EchoServer server;
	if (!server.set_config(cfg)) {
		return EXIT_FAILURE;
	}

	pthread_t server_thread;
	pthread_create(&server_thread, NULL, SocketsServer::run_server, &server);

	std::string cmd;
	while (std::cin >> cmd) {
		if (cmd == "exit") {
			if (!server.stop()) {
				return EXIT_FAILURE;
			}
			break;
		}
	}

	pthread_join(server_thread, NULL);
	return EXIT_SUCCESS;
}
