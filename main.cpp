/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#include <stdlib.h>

#include <sstream>
#include <iostream>

#include "args.h"
#include "config.h"
#include "socketserver.h"


int main(int argc, char* argv[]) {
	Args args(argc, argv);

	printf("Will load config from %s.\n", args.config.c_str());
	Config cfg;
	if (!cfg.read_config()) {
		return EXIT_FAILURE;
	}

	SocketsServer server;
	if (!server.set_config(cfg)) {
		return EXIT_FAILURE;
	}

	// move to separate thread
	if (!server.run()) {
		return EXIT_FAILURE;
	}

	if (!server.stop()) {
		return EXIT_FAILURE;
	}

	// join server thread

	return EXIT_SUCCESS;
}
