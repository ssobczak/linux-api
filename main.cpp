#include <stdio.h>
#include <stdlib.h>

#include "args.h"

using namespace std;

int main(int argc, char* argv[]) {
	Args args(argc, argv);

	printf("Will load config from %s.\n", args.config.c_str());

	return EXIT_SUCCESS;
}
