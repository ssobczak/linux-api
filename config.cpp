/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#include "config.h"

Config::Config() {
}

Config::~Config() {
}

bool Config::read_config() {
	port = "6543";
	backlog = 10;

	return true;
}

