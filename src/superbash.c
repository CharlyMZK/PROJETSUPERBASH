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
#define pipe_separator '|'
#define and_separator '&'


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
  return true;
}


bool is_separator(char instruction){
 printf("[Log] Is this a command ? (%c)\n",instruction); 
 
 if((instruction == pipe_separator) || (instruction == and_separator)){
   printf("Hey ! Its a pipe !?\n");
   return true;
 }

  return false;
}

/*void create_tree_from_command_(char* command){
  printf("[Log] Creating tree \n");
  int end = strlen(command) - 1; 
  int index = end;
  int lastSeparatorPosition = 0;
  Node* root = NULL;
  Node* actualUsedNode = NULL;
  Node* newNode = NULL;
  
  char* separator;

  
  printf("[The command is : %s | I'm going to read > end : %d index : %d]\n",command, end,index);
  
  
  while (index > 0) {
    printf("[Read] letter checked : %c \n", command[index]);
    if(is_separator(command[index])){
      lastSeparatorPosition = index;
      separator = substr(command,index,2);
      printf("Separator found : %s",separator);
      
      printf("[Separator] I found a separator, creating a node from %d to %d\n", index+1, end);
      if(actualUsedNode == NULL){ 
        root = create_root(substr(command,index,2),NULL,create_root(substr(command,index+1,end),NULL,NULL)); 
        actualUsedNode = root;
      }else{
        newNode = create_root(substr(command,index,index-1),NULL,create_root(substr(command,index+1,end),NULL,NULL));
        if(actualUsedNode->leftChild == NULL){
          actualUsedNode->leftChild = newNode;
          actualUsedNode = newNode;
        }
      }
      index--;
    }
    index --;
  }
  
  if(root == NULL){
    printf("[No root] Root is null, only one command, creating..\n");
    root = create_root(command,NULL,NULL); 
  }else if(actualUsedNode->leftChild == NULL){
    actualUsedNode->leftChild = create_root(substr(command,0,lastSeparatorPosition),NULL,NULL);
  }else if(actualUsedNode->rightChild == NULL){
    actualUsedNode->rightChild = create_root(substr(command,0,lastSeparatorPosition),NULL,NULL);
  }
  
  printf("[Root] Root command : %s\n",root->command);
  printf("Printing prefix : \n");
  print_prefix(root);
  printf("\n");
}*/


void create_tree_from_command_(char* command){
  printf("[Log] Creating tree \n");
  int end = strlen(command) - 1; 
  int index = end;
  int lastSeparatorPosition = end+1;
  Node* root = NULL;
  Node* actualUsedNode = NULL;
  Node* newNode = NULL;
  int truncateLength;
  char* separator;
  //char* afterSeparatorFoundCommand;
  char* truncatedCommand;
  
  printf("[The command is : %s | I'm going to read > end : %d index : %d]\n",command, end,index);
  
  
  while (index > 0) {
    printf("[Read] letter checked : %c \n", command[index]);
    if(is_separator(command[index])){
      separator = substr(command,index-1,2);
      truncateLength = lastSeparatorPosition - (index+1);
      truncatedCommand = substr(command,index+1,truncateLength);
      printf("[Separator] Truncate length = %d - %d\n",lastSeparatorPosition,(index+1));
      printf("[Separator] Separator found : %s\n",separator);
      printf("[Separator] Truncate length : %d\n",truncateLength);  
      printf("[Separator] Truncated command from %d to %d\n",index+1,truncateLength);  
      printf("[Separator] Creating a node from %d for %d caracts\n", index+1, truncateLength);
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
      printf("[Separator Position] Separator position : %d\n",lastSeparatorPosition);
    }
    index --;
    printf("[ ROW ]");
  }
  
  if(root == NULL){
    printf("[No root] Root is null, only one command, creating..\n");
    root = create_root(command,NULL,NULL); 
  }else if(actualUsedNode->leftChild == NULL){
    actualUsedNode->leftChild = create_root(substr(command,0,lastSeparatorPosition),NULL,NULL);
  }else if(actualUsedNode->rightChild == NULL){
    actualUsedNode->rightChild = create_root(substr(command,0,lastSeparatorPosition),NULL,NULL);
  }
  
  printf("[Root] Root command : %s\n",root->command);
  printf("Printing prefix : \n");
  print_prefix(root);
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
	bash_loop();
	//create_tree_from_command(17,"ls -l | grep 2048");
	return EXIT_SUCCESS;
}