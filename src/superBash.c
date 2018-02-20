#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "commandExecutor.h"

int main(int argc, char *argv[])
{ 
	bash_loop();
	return EXIT_SUCCESS;
}