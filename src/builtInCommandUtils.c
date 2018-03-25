/**
 * @file
 * contient les fonctions relative aux commande built in
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "treeUtils.h"
#include "stringUtils.h"
#include "../include/builtInCommandUtils.h"


/**
 * Affiche le répertoire courant
 */
void print_current_directory()
{ 
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
   fprintf(stdout, "Current working dir: %s\n", cwd);
}

/**
 * Change le répertoire courant
 */
void change_current_directory(char *path)
{
  printf("changing current directory to %s \n",path);
  chdir(path);
  print_current_directory();
}

/**
 * Affiche ce qui se situe après echo dans node->command
 */
void echo(Node * node)
{
  int len = strlen(node->command);
  char * copyCommand = malloc(sizeof(char) * len);
  strcpy(copyCommand,node->command);
  copyCommand = substr(copyCommand,4,len-3);
  printf("%s\n",copyCommand);
}
