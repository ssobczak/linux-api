/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#include <stdlib.h>

#include <sstream>
#include <string>
#include <iostream>
#include <thread>

#include "args.h"
#include "config.h"
#include "dispatchserver.h"
#include "socketserver.h"
#include "workerspool.h"

int main(int argc, char* argv[]) {
	Args args(argc, argv);

	printf("Will load config from %s.\n", args.config.c_str());
	Config cfg;
	if (!cfg.read_config()) {
		return EXIT_FAILURE;
	}

	WorkersPool wp(10);

	DispatchServer server(wp);
	if (!server.set_config(cfg)) {
		return EXIT_FAILURE;
	}

	std::thread server_thread;
	if (!server.spawn(&server_thread)) {
		return EXIT_FAILURE;
	}

	std::string cmd;
	while (std::cin >> cmd) {
		if (cmd == "exit") {
			if (!server.stop()) {
				return EXIT_FAILURE;
			}
			break;
		}
	}

	server_thread.join();
	return EXIT_SUCCESS;
}
