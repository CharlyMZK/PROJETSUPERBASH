/**
 * @file
 * contient les fonctions permettant de créer et de lire l'arbre
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<errno.h>
#include <unistd.h>
#include "../include/treeUtils.h"
#include "../include/commandHandler.h"
#include "../include/fileUtils.h"
#include "../include/commandExecutor.h"
#include "../include/logUtils.h"
#include "../include/stringUtils.h"
#define LSH_RL_BUFSIZE 1024
#define OUTPUT_FILEPATH "/tmp/tmpOutputFile"
#define INPUT_FILEPATH "/tmp/tmpInputFile"
#define LOG_FILEPATH "/tmp/command.txt"

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
  log_message("CommandHandler.create_tree_from_command","Creating tree..");
  log_string("CommandHandler.create_tree_from_command","Command read is",command);
  
  // Note : we are reading command from end to begining
  
  while (index > 0) { // Reading command
    log_message("CommandHandler.create_tree_from_command","Row starded");
    log_char_value("CommandHandler.create_tree_from_command","Letter checked",command[index]);
    if(is_separator(command[index])){
      // If caracter get is a separator, truncating command in two string, the first one ( separator ) with the separator string
      // And the other one with the truncated command after the separator
      log_message("CommandHandler.create_tree_from_command","Getting separator..");
      separator = substr(command,index-1,2);
      truncateLength = lastSeparatorPosition - (index+1);
      log_message("CommandHandler.create_tree_from_command","Getting command..");
      // If there's a separator and length is > 0
      if(truncateLength > 0){
        // Getting after separator command
        truncatedCommand = substr(command,index+1,truncateLength);
        remove_space_at_beginning_and_end(truncatedCommand);
        log_string("CommandHandler.create_tree_from_command","Separator found",separator);
        log_value("CommandHandler.create_tree_from_command","Truncating after separator with length",truncateLength);
      }else{
        // Asking the after separator command
        log_string("CommandHandler.create_tree_from_command","Separator found",separator);
        log_message("CommandHandler.create_tree_from_command","But length is 0");
        printf(">");
        truncatedCommand = read_console_line();
      }
      
      // Then creates a node with both
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
      
      // Decreasing index and saving the last index position to truncate from the next separator to this position
      index--;
      lastSeparatorPosition = index;
      log_value("CommandHandler.create_tree_from_command","Last separator position",lastSeparatorPosition);
    }
    index --;
    log_message("CommandHandler.create_tree_from_command","Row ended");
  }
  log_message("CommandHandler.create_tree_from_command","While ended,  finishing the three..");

  // Command read ended, handling first part of command
  char *firstCommand = malloc(lastSeparatorPosition + 1);
  if(root == NULL){
    log_message("CommandHandler.create_tree_from_command","Root is null, only one command, creating..");
    remove_space_at_beginning_and_end(command);
    root = create_root(command,NULL,NULL); 
  }else if(actualUsedNode->leftChild == NULL){
    log_message("CommandHandler.create_tree_from_command","Creating last left child");
    log_value("CommandHandler.create_tree_from_command","lastSeparatorPosition",lastSeparatorPosition);
    firstCommand[lastSeparatorPosition] = '\0';
    firstCommand = substr(command,0,lastSeparatorPosition);
    actualUsedNode->leftChild = create_root(firstCommand,NULL,NULL);
  }else if(actualUsedNode->rightChild == NULL){
    log_message("CommandHandler.create_tree_from_command","Creating last right child");
    log_value("CommandHandler.create_tree_from_command","lastSeparatorPosition",lastSeparatorPosition);
    firstCommand[lastSeparatorPosition] = '\0';
    firstCommand = substr(command,0,lastSeparatorPosition);
    actualUsedNode->rightChild = create_root(substr(firstCommand,0,lastSeparatorPosition),NULL,NULL);
  }
  log_string("CommandHandler.create_tree_from_command","Root command",root->command);
  log_string("CommandHandler.create_tree_from_command","Root separator",root->separator);
  log_message("CommandHandler.create_tree_from_command","Tree created.");
  log_message("CommandHandler.create_tree_from_command","Printing prefix..");
  log_tree(root);
  return root;
}

/**
 * Demande à l'utilisateur d'entrer du texte sur l'invit de commande
 * stock le texte entré dans le noeud
 */
bool read_entered_parameters(Node* treeCommand){
  FILE *out = fopen(INPUT_FILEPATH, "a+");  
  char enteredString[255]= "";
  char command[20];
  bool isRunning = true;
  log_message("CommandHandler.read_and_exec_tree","Its a <<");  
  log_value("CommandHandler.read_and_exec_tree","Comparing string value",strcmp(enteredString,treeCommand->rightChild->command));
  log_string("CommandHandler.read_and_exec_tree","Enter value until",treeCommand->rightChild->command);
  do {
    printf("> ");
    fgets(command, sizeof command, stdin);
    remove_newline_ch(command);
    if(strcmp(command,treeCommand->rightChild->command) == 0){
      log_message("CommandHandler.read_and_exec_tree","Equality found, exit");  
      isRunning = false;
    }else{
      fprintf(out, "%s\n", command);
      log_value("CommandHandler.read_and_exec_tree","Comparing sting value in while",strcmp(command,treeCommand->rightChild->command));
      log_string("CommandHandler.read_and_exec_tree","Enter value until",treeCommand->rightChild->command);
    }
  } while (isRunning);
  fclose(out);
  return true;
}


/**
 * Lis un arbre et execute les commandes 
 */
bool read_and_exec_tree(Node* treeCommand){
  log_message("CommandHandler.read_and_exec_tree","Reading and executing command");
  // Checking if there is a < separator
  if(treeCommand->separator != NULL && (treeCommand->separator[1] == lower_separator)){
    if(treeCommand->separator[0] == lower_separator){
         read_entered_parameters(treeCommand);
    }else{
      log_message("CommandHandler.read_and_exec_tree","< found, copying his file into input");
      log_string("CommandHandler.read_and_exec_tree","His separator",treeCommand->rightChild->command);
      switch_from_file_content_to_file(treeCommand->rightChild->command,INPUT_FILEPATH);
    } 
  }
  
  // Started executing tree
  if(treeCommand->leftChild != NULL){
    // Start execution recursively by left child
    if(read_and_exec_tree(treeCommand->leftChild)){
      log_message("CommandHandler.read_and_exec_tree","Leftchild is not null"); 
      // There is a command, executing it
      if(treeCommand->command != NULL){
        log_string("CommandHandler.read_and_exec_tree","(Command not null ) Executing command",treeCommand->command);
        log_string("CommandHandler.read_and_exec_tree","(Both childs Null) Input value :",treeCommand->inputValue);
        handle_command(treeCommand);
        if(treeCommand->result != NULL && treeCommand->command != NULL){
          log_string("CommandHandler.read_and_exec_tree","Command was",treeCommand->command);
          log_string("CommandHandler.read_and_exec_tree","Result is ",treeCommand->result);
        }
      // Checking if there is a separator instead and handling it
      // Note : Once then left child is executed, we execute the right one
      }else{
        // Handling < 
        if(treeCommand->separator != NULL && (treeCommand->separator[1] == lower_separator)){
           log_message("CommandHandler.read_and_exec_tree","Rightchild command is a file, he's not executed.");
           return true;
         }
         // Handling ;
         if(treeCommand->separator != NULL && (treeCommand->separator[1] == continue_separator)){
           remove_space_at_beginning_and_end(treeCommand->separator);
           treeCommand->rightChild->inputValue = treeCommand->separator;
           display_file_content(OUTPUT_FILEPATH);
           log_message("CommandHandler.read_and_exec_tree","Separator is continue, going to right child");
           return read_and_exec_tree(treeCommand->rightChild);
         }
         // Handling &&
         if(treeCommand->separator != NULL && (treeCommand->separator[0] == and_separator) && (treeCommand->separator[1] == and_separator)){
           log_message("CommandHandler.read_and_exec_tree","Its a && separator, handling it..");
           log_value("CommandHandler.read_and_exec_tree","Previous command successed ?",treeCommand->leftChild->success);
           if(treeCommand->leftChild->success){
              treeCommand->success = true;
              log_message("CommandHandler.read_and_exec_tree","Yes, executing following");
              display_file_content(OUTPUT_FILEPATH);
              remove_space_at_beginning_and_end(treeCommand->separator);
              treeCommand->rightChild->inputValue = treeCommand->separator;
              //switch_from_file_content_to_file(OUTPUT_FILEPATH,INPUT_FILEPATH);
              return read_and_exec_tree(treeCommand->rightChild);
           }else{
             treeCommand->success = false;
             return true;
           }
         }
         // Handling ||
         if(treeCommand->separator != NULL && (treeCommand->separator[0] == pipe_separator) && (treeCommand->separator[1] == pipe_separator)){
            log_message("CommandHandler.read_and_exec_tree","Its a || separator, handling it..");
            log_value("CommandHandler.read_and_exec_tree","Previous command successed ?",treeCommand->leftChild->success);
            if(treeCommand->leftChild->success){
            treeCommand->success = false;
            return true;
           }else{
             treeCommand->success = true;
              log_message("CommandHandler.read_and_exec_tree","Yes, executing following");
              display_file_content(OUTPUT_FILEPATH);
              remove_space_at_beginning_and_end(treeCommand->separator);
              treeCommand->rightChild->inputValue = treeCommand->separator;
              //switch_from_file_content_to_file(OUTPUT_FILEPATH,INPUT_FILEPATH);
              return read_and_exec_tree(treeCommand->rightChild);
           }
         }
        // Common separator treatments
        log_message("CommandHandler.read_and_exec_tree","Command is null, its a separator");
        remove_space_at_beginning_and_end(treeCommand->separator);
        treeCommand->rightChild->inputValue = treeCommand->separator;
        switch_from_file_content_to_file(OUTPUT_FILEPATH,INPUT_FILEPATH);
        return read_and_exec_tree(treeCommand->rightChild);
      }
    }
  // Handling the last node of the tree ( no childs left )  
  }else if(treeCommand->leftChild == NULL && treeCommand->rightChild == NULL){
    log_string("CommandHandler.read_and_exec_tree","(Both childs Null) Executing command",treeCommand->command);
    log_message("CommandHandler.read_and_exec_tree","Avant display ");
  
    bool isExecuted = 0;
    isExecuted = handle_command(treeCommand);
    log_message("CommandHandler.read_and_exec_tree","This node is at the end of three, command executed.");
    if(treeCommand->result != NULL && treeCommand->command != NULL){
      log_string("CommandHandler.read_and_exec_tree","Command was",treeCommand->command);
    }
    return isExecuted;
  }
  return true;
}

/**
 * Vérifie si le fork à réussi
 */
void checkIfForkSuccessed(int forkReturnValue){
   if(forkReturnValue == -1){
       dprintf(1,"\nException occured : ");
       perror(strerror(errno));
       exit(0);
   }
}

/**
 * Analyse la commande passé en paramètre et l'execute
 */
int create_and_execute_tree(char* command){
  // Logging command (in file)
  log_in_file(command,LOG_FILEPATH);
  log_message("CommandHandler.handle_command","Handling command..");
  
  remove_space_at_beginning_and_end(command);
  
  //Check if the command has to be forked
  bool background = false;
  int indexAnd = string_contain_and_at_end(command);
  if(indexAnd != -1)
  {
    command[indexAnd] = '\0';
    remove_space_at_beginning_and_end(command);
    log_string("CommandHandler.create_and_execute_tree","executing in background command :",command);
    background = true;
  }
  Node* treeCommand = create_tree_from_command(command);
  log_message("CommandHandler.handle_command","Reading and executing tree..");
  
  int forkId = -1;
  if(background)
  {
    log_string("CommandHandler.create_and_execute_tree","lancement de la commande en arrière plan :",command);
    forkId = fork();
    checkIfForkSuccessed(forkId);
    if(forkId)
      return true;
  }
  
  read_and_exec_tree(treeCommand);
  display_file_content(OUTPUT_FILEPATH);
  if(background && forkId == 0)
  {
    exit(EXIT_SUCCESS);
  }
  log_message("CommandExecutor.handle_command","Cleaning files..");
  empty_file(OUTPUT_FILEPATH);
  clean(treeCommand);
  return true;
}

