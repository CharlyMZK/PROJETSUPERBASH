#include "treeUtils.h"
#include "commandHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#define LSH_RL_BUFSIZE 1024

/**
 * Lis une ligne de la console et renvois la ligne lu
 */
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

/**
 * Execute les commandes passé par ligne de commande
 */
void bash_loop(void)
{
  char *command;
  bool isRunning = true;
  printf("- Prompt launched, Saltscript only please. -\n");
  do {
    printf("Prompt > ");
    command = read_console_line();
    isRunning = create_and_execute_tree(command);
    //isRunning = executeCommand(command);
  } while (isRunning);
  free(command);
}


int main(int argc, char *argv[])
{ 

	bash_loop();
	return EXIT_SUCCESS;
}