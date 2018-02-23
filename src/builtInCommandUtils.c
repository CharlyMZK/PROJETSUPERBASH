#include <stdio.h>
#include <unistd.h>
#include "builtInCommandUtils.h"

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
 * Affiche ECHO
 * TODO passé des paramètres
 */
void echo()
{
  printf("echo\n");
}

