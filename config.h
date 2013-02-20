/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#include <string>

#ifndef CONFIG_H_
#define CONFIG_H_

class Config {
public:
	Config();
	virtual ~Config();

	std::string port;
};

#endif /* CONFIG_H_ */
