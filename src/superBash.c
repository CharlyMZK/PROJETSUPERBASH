/**
 * @file
 * contient le main et les fonctions de lecture de ligne passé par l'utilisateur
 */
 
#include "../include/treeUtils.h"
#include "../include/commandHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stringUtils.h>
#include <unistd.h>
#define LSH_RL_BUFSIZE 1024

/**
 * Récupère les arguments
 */
  char *read_args(void)
{
  int bufsize = LSH_RL_BUFSIZE;
  int position = 0;
  char *bf = malloc(sizeof(char) * bufsize);
  int c;
  while (1) {
    // Read a character
    c = getchar();
    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      bf[position] = '\0';
      return bf;
    } else {
      bf[position] = c;
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
     if(!isEmptyString(command)){
       isRunning = create_and_execute_tree(command);
     }
      
    //isRunning = executeCommand(command);
  } while (isRunning);
  free(command);
}

/**
 * Point d'entrée de l'application
 */ 
int main(int argc, char *argv[])
{ 
  if(argv[1] == NULL){
	  bash_loop();
  }else if(!strcmp (argv[1], "-c")){
    create_and_execute_tree(argv[2]);
  }else if(!ifStringContainsHyphen(argv[1])){
    printf("\nWe don't execute any binary files, sorry !\n");
  }else if(ifStringContainsHyphen(argv[1])){
    printf("\nWe don't handle bash options sorry !\n");
  }
	return EXIT_SUCCESS;
}