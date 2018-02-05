#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "tree.h"
#define LSH_RL_BUFSIZE 1024



char *read_console_line(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;
  while (1) {
    // Read a character
    c = getchar();
    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;
  }
}

void bash_loop(void)
{
  char **command;
  bool isRunning = true;
  do {
    printf("> ");
    command = read_console_line();
    isRunning = execute_command(command);
  } while (isRunning);
  free(command);
}

int execute_command(char **command)
{
  int i;
  if (command[0] == NULL) {
    return false;
  }else{
  	return true;
  }
}

void printCurrentDirectory()
{
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
   fprintf(stdout, "Current working dir: %s\n", cwd);
}

void changeCurrentDirectory(char *path)
{
  printf("changing current directory to %s \n",path);
  chdir(path);
  printCurrentDirectory();
}

int main(int argc, char *argv[])
{
	//create_tree_from_command(argc,argv);
	bash_loop();
	return EXIT_SUCCESS;
}