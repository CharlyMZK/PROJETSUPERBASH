#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tree.h"

int main(int argc, char *argv[]);
char *read_console_line(void);

void bash_loop(void);



int execute_command(char **command);

void changeCurrentDirectory(void);
