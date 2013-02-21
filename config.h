/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stddef.h>
#include <string>

class Config {
public:
	Config();
	virtual ~Config();

	std::string port;
	int backlog;

	bool read_config();
};

#endif /* CONFIG_H_ */
