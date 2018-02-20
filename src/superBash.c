#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "commandExecutor.h"

int main(int argc, char *argv[])
{ 
	char* test1 = "ls | ls";
	char* test3 = substr(test1,0,3);
	char* test2 = substr(test1,5,3);
	printf("%s, %s \n", test3, test2);
	remove_space_at_beginning_and_end(test3);
	printf("%s, %s \n", test3, test2);


	system(test3);
	system(test2);
	bash_loop();
	return EXIT_SUCCESS;
}