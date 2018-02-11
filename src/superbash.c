/**
 * \file Superbash.c
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
#include "logger.h"
#define LSH_RL_BUFSIZE 1024
#define pipe_separator '|'
#define and_separator '&'
#define VERBOSE true

char *substr(char *src,int pos,int len) { 
  char *dest=NULL;                        
  if (len>0) {                            
    dest = (char *) malloc(len+1);        
    strncat(dest,src+pos,len);            
  }                                       
  return dest;                            
}


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

/**
 * Enlève les espaces en début de chaine
 */
void trimLeading(char * str)
{
    int index, i, j;

    index = 0;

    /* Trouve le dernier index des premiers espaces */
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index++;
    }
    if(index != 0)
    {
        /* shift le tableau sur la gauche */
        i = 0;
        while(str[i + index] != '\0')
        {
            str[i] = str[i + index];
            i++;
        }
        str[i] = '\0'; //Ferme la string
    }
}
/**
 * Enlève les espaces en fin de chaine
 * 
 */
void trimLast(char * str)
{
    int index, i, lastIndex;
    lastIndex = strlen(str) -1;
    index = lastIndex;
    /* Trouve le premier index des derniers espaces */
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index--;
    }
    if(index != lastIndex )
    {
        str[index] = '\0'; // Make sure that string is NULL terminated
    }
}


void remove_space_at_beginning_and_end(char * string)
{
  trimLeading(string);
  printf("%s fin de la string\n", string);
  trimLast(string);
  printf("%s fin de la string\n", string);
}

int handle_command(char* command){
  printf("[Log] Handling command \n");
  create_tree_from_command(command);
  return true;
}


bool is_separator(char instruction){
 log_char_value("Superbash.is_separator","Is this a command ?",instruction); 
 if((instruction == pipe_separator) || (instruction == and_separator)){
   log_message("Superbash.is_separator","Separator found");
   return true;
 }

  return false;
}

void create_tree_from_command(char* command){
  
  int end = strlen(command) - 1; 
  int index = end;
  int lastSeparatorPosition = end+1;
  Node* root = NULL;
  Node* actualUsedNode = NULL;
  Node* newNode = NULL;
  int truncateLength;
  char* separator;
  char* truncatedCommand;
  log_message("Superbash.create_tree_from_command","Creating tree..");
  log_string("Superbash.create_tree_from_command","Command read is",command);
  
  while (index > 0) {
    log_message("Superbash.create_tree_from_command","Row starded");
    log_char_value("Superbash.create_tree_from_command","Letter checked",command[index]);
    if(is_separator(command[index])){
      separator = substr(command,index-1,2);
      truncateLength = lastSeparatorPosition - (index+1);
      truncatedCommand = substr(command,index+1,truncateLength);
      log_char_value("Superbash.create_tree_from_command","Separator found",separator);
      log_value("Superbash.create_tree_from_command","Truncate length",truncateLength);
      if(actualUsedNode == NULL){ 
        root = create_root(separator,NULL,create_root(truncatedCommand,NULL,NULL)); 
        actualUsedNode = root;
      }else{
        newNode = create_root(separator,NULL,create_root(truncatedCommand,NULL,NULL));
        if(actualUsedNode->leftChild == NULL){
          actualUsedNode->leftChild = newNode;
          actualUsedNode = newNode;
        }
      }
      index--;
      lastSeparatorPosition = index;
      log_value("Superbash.create_tree_from_command","Last separator position",lastSeparatorPosition);
    }
    index --;
    log_message("Superbash.create_tree_from_command","Row ended");
  }
  
  //remove_space_at_beginning_and_end(command);
  if(root == NULL){
    log_message("Superbash.create_tree_from_command","Root is null, only one command, creating..");
    root = create_root(command,NULL,NULL); 
  }else if(actualUsedNode->leftChild == NULL){
    actualUsedNode->leftChild = create_root(substr(command,0,lastSeparatorPosition),NULL,NULL);
  }else if(actualUsedNode->rightChild == NULL){
    actualUsedNode->rightChild = create_root(substr(command,0,lastSeparatorPosition),NULL,NULL);
  }
  
  log_string("Superbash.create_tree_from_command","Root command",root->command);

  printf("Printing prefix : \n");
  print_prefix(0,root);
  printf("\n");
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
  printf(" test trim \n");
  char ex[] ="                                    test avec espace avant et des cara spéciaux || && <                             ";
  //char * ex = "                                   test avec espace avant et des cara spéciaux || && <  ";
  remove_space_at_beginning_and_end(ex);
	bash_loop();
	//create_tree_from_command(17,"ls -l | grep 2048");
	return EXIT_SUCCESS;
}