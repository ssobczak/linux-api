/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#include <stdlib.h>

#include <sstream>
#include <iostream>

#include "args.h"
#include "config.h"
#include "server.h"


int main(int argc, char* argv[]) {
	Args args(argc, argv);

	printf("Will load config from %s.\n", args.config.c_str());
	Config cfg;
	if (!cfg.read_config()) {
		return EXIT_FAILURE;
	}

	Server server;
	if (!server.start(cfg)) {
		return EXIT_FAILURE;
	}

	while (true) {
		int client = server.accept_client();

		std::stringstream msg;
		msg << client << " has joined!" << std::endl;
		std::cout << msg.str();

		server.broadcast(msg.str());
	}


	return EXIT_SUCCESS;
}
