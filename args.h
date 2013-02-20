/*
 *  Created on: Feb 20, 2013
 *      Author: ssobczak
 */

#include <string>
#include <cstdio>

#ifndef ARGS_H_
#define ARGS_H_

class Args {
public:
	bool daemon;
	std::string config;
	std::string program_name;

	Args(int argc, char* argv[]);
	virtual ~Args();

	void print_usage(FILE* stream) const;

private:
	void parse_args(int argc, char* argv[]);
};

#endif /* ARGS_H_ */
