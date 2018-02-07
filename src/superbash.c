/**
 * \file superbash.c
 * \brief Programme principal du projet
 * \author Charly Mrazeck, Baptiste Oberbach
 * \version 0.1.0
 * \date février 2018
 *
 * Programme executant le bash du projet superbash
 *
 */


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
  char *command;
  bool isRunning = true;
  printf("- Prompt launched, Saltscript only please. -\n");
  do {
    printf("Prompt > ");
    command = read_console_line();
    isRunning = handle_command(command);
    //isRunning = executeCommand(command);
  } while (isRunning);
  free(command);
}

/*
int executeCommand(char **command)
{
  int i;
  if (command[0] == NULL) {
    return false;
  }else{
  	return true;
  }
}*/

int handle_command(char* command){
  printf("[Log] Handling command \n");
  create_tree_from_command_(command);
  int bufsize = LSH_RL_BUFSIZE;
  char *ptr = malloc(sizeof(char) * bufsize);
  ptr=strstr(command,"&&");
  int pos = ptr - command;
  printf("Pos %d",pos);
  
  
  return true;
}


void create_tree_from_command_(char* command){
  printf("[Log] Creating tree \n");
}


int execute_command(char * command)
{
  system(command);
  return true;
}

void print_current_directory()
{
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
   fprintf(stdout, "Current working dir: %s\n", cwd);
}

void change_current_directory(char *path)
{
  printf("changing current directory to %s \n",path);
  chdir(path);
  print_current_directory();
}

int main(int argc, char *argv[])
{
	bash_loop();
	create_tree_from_command(17,"ls -l | grep 2048");
	return EXIT_SUCCESS;
}