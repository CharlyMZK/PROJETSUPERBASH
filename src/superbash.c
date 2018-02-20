/**
 * @file
 * @brief Programme principal du projet
 * @author Charly Mrazeck, Baptiste Oberbach
 * @version 0.1.0
 * @date février 2018
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

/**
 * retourne un string à partir de la string src à la position pos pour une longueur len 
 */
char *substr(char *src,int pos,int len) { 
  char *dest=NULL;                        
  if (len>0) {                            
    dest = (char *) malloc(len+1);        
    strncat(dest,src+pos,len);            
  }                                       
  return dest;                            
}

/**
 * retourne l'index de la première occurence du caractère charToFind dans la string src
 * ou -1 si le charactère n'est pas présent
 */
int *findIndexOffFirstOccurenceInString(char *src, char charToFind)
{
  const char *ptr = strchr(src, charToFind);
  if(ptr) 
   return ptr - src;
  else
    return -1;
}

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
 * Log la commande command à la fin du fichier file
 */
bool logInFile(char * command, char * file)
{
  FILE * pFile = fopen(file, "a");
  if(pFile == NULL)
  {
    printf("Error opening file, impossible to log command \n");
    return;
  }
  fprintf(pFile, command);
  fprintf(pFile, "\n");
  fclose(pFile);
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

/**
 * Enlève les espaces en début et en fin de chaine
 */
void remove_space_at_beginning_and_end(char * string)
{
  trimLeading(string);
  trimLast(string);
}




/**
 * Créer un arbre à partir de la commande passé en paramètre
 */
Node* create_tree_from_command(char* command){
  
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
  
  return root;
}

/**
 * Lis un arbre et execute les commandes 
 */
bool read_and_exec_tree(Node* treeCommand){
  log_message("Superbash.read_and_exec_tree","Reading and executing command");
  if(treeCommand->leftChild != NULL){
    if(read_and_exec_tree(treeCommand->leftChild)){
      log_message("Superbash.read_and_exec_tree","Leftchild is not null");  
      if(treeCommand->command != NULL){
        log_string("Superbash.read_and_exec_tree","(Command not null ) Executing command",treeCommand->command);
        execute_command(treeCommand->command);
      }else{
        log_message("Superbash.read_and_exec_tree","Command is null, its a separator");
        return read_and_exec_tree(treeCommand->rightChild);
      }
    }
  }else if(treeCommand->leftChild == NULL && treeCommand->rightChild == NULL){
    log_string("Superbash.read_and_exec_tree","(Null & Null ) Executing command",treeCommand->command);
    return execute_command(treeCommand->command);
  }
  return true;
}

/**
 * Analyse la commande passé en paramètre et l'execute
 */
int handle_command(char* command){
  log_message("Superbash","Handling command..");
  Node* treeCommand = create_tree_from_command(command);
  log_message("Superbash","Reading and executing tree..");
  read_and_exec_tree(treeCommand);
  return true;
}

/**
 * Execute une commande 
 */
int execute_command(char * command)
{
  //Log de la commande 
  logInFile(command,"./command.txt");
  char * commandCore;
  char * parameters;
  int indexEndOffCommand = findIndexOffFirstOccurenceInString(command,' ');
  //Separate the core of the command from its parameter
  if(indexEndOffCommand != -1)
  {
    commandCore = substr(command,0,indexEndOffCommand);
    parameters = command + indexEndOffCommand + 1;
  }
  else 
    commandCore = command;

  //Exeucting custom command
  if(!strcmp(commandCore, "pwd"))
    print_current_directory();
  else if(!strcmp(commandCore, "cd"))
    change_current_directory(parameters);
  else if(!strcmp(commandCore, "echo"))
    printf("%s\n",parameters);
  else if(!strcmp(commandCore, "exit"))
    //TODO EXIT
    exit(EXIT_SUCCESS);
  else 
  {
    printf("%d \n",strcmp(commandCore, "cd"));
    log_message("Superbash","executing using the system fonction");
    system(command);
  }
    
  return true;
}

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

int main(int argc, char *argv[])
{
	bash_loop();
	//create_tree_from_command(17,"ls -l | grep 2048");
	return EXIT_SUCCESS;
}