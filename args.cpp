/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#include "args.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

Args::Args(int argc, char* argv[]) : daemon(false), config(""), program_name("") {
	parse_args(argc, argv);
}

Args::~Args() {
}

void Args::print_usage(FILE* stream) const {
	fprintf(stream, "Usage: %s options.\n"
			"	-c --config	config_file Load config from config_file.\n"
			"	-d --daemon				Don't run as daemon.\n"
			"	-h --help				Display this information.\n",
			program_name.c_str());
}

void Args::parse_args(int argc, char* argv[]) {
	if (argc < 1) {
		abort();
	}

	program_name = argv[0];

	const char* short_options = "hc:d";
	const struct option long_options[] = {
		{"help", 0, NULL, 'h'},
		{"config", 1, NULL, 'c'},
		{"daemon", 0, NULL, 'd'},
		{NULL, 0, NULL, 0}
	};

	int next_option = -1;
	do {
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);

		switch (next_option) {
		case 'h':
			print_usage(stdout);
			exit(EXIT_SUCCESS);
		case 'd':
			daemon = true;
			break;
		case 'c':
			config = optarg;
			break;
		case '?':
			print_usage(stderr);
			exit(EXIT_FAILURE);
		case -1:
			break;
		default:
			abort();
		}
	} while (next_option != -1);

	if (config == "") {
		print_usage(stderr);
		exit(EXIT_FAILURE);
	}
}

