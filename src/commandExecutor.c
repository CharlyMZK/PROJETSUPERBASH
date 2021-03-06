/**
 * @file
 * contient les fonctions relative à l'écriture de log
 */

#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<errno.h>
#include <stdbool.h>
#include "../include/treeUtils.h"
#include "../include/builtInCommandUtils.h"
#include "../include/aliasUtils.h"
#include "../include/commandExecutor.h"
#include "../include/commandHandler.h"
#include "../include/logUtils.h"
#include "../include/stringUtils.h"
#include "../include/fileUtils.h"
#define LSH_RL_BUFSIZE 1024
#define OUTPUT_FILEPATH "/tmp/tmpOutputFile"
#define INPUT_FILEPATH "/tmp/tmpInputFile"

/**
 * renvoie un tableau de string contenant la commande et ses paramètres 
 */
char** build_command(Node * node){
  char** splitedBySpacesCommand;
  char * copyCommand = malloc(sizeof(char) * strlen(node->command));
  strcpy(copyCommand,node->command);
  if(!is_file_empty(INPUT_FILEPATH)){
    splitedBySpacesCommand = str_split_and_add_path(copyCommand,INPUT_FILEPATH);
    log_message("CommandExecutor.executeCommand","Input file isnt empty.");
  }else{
    splitedBySpacesCommand = str_split(copyCommand, ' ');
    log_string("CommandExecutor.executeCommand","First token : ",splitedBySpacesCommand[0]);
  }
  return splitedBySpacesCommand;
}

/**
 * Copie dans input_filepath le fichier passé dans la commande
 */
bool write_node_in_file(Node* node){
  log_message("CommandExecutor.executeCommand","Ecriture dans un nouveau fichier..");
  return switch_from_file_content_to_file(INPUT_FILEPATH,node->command);
}

/**
 * Ajoute dans input_filepath le fichier passé dans la commande
 */
bool append_node_in_file(Node* node){
  log_message("CommandExecutor.executeCommand","Ajout a la suite du fichier..");
  return append_from_file_content_to_file(INPUT_FILEPATH,node->command);
}

/**
 * Return true si le séparateur est >
 */
bool is_separator_redirecting_ouput(Node* node){
  log_message("CommandHandler.handle_command","Checking if a seperator want to redirect output");
  return node->inputValue != NULL && (node->inputValue[0] == higher_separator); 
}

/**
 *  Return true si le séparateur est >>
 */
bool is_separator_appending_a_file(Node* node){
  log_message("CommandHandler.handle_command","Checking if a seperator want to append to a file");
  if(node->inputValue == NULL){
   log_message("CommandHandler.handle_command","Input value null");
  }else{
    log_message("CommandHandler.handle_command","Not null");
  }
  return node->inputValue != NULL && (node->inputValue[0] == higher_separator && node->inputValue[1]== higher_separator); 
}

/**ls
 * Execute une commande 
 */
int handle_command(Node* node)
{
  int execReturnValue = 0;
  bool isRedirected = false;
  bool useExecvp = false;
  log_message("CommandHandler.handle_command","Handling command, checking if < or <<");
  // If the node got < or << we are redirecting the output instead of executing command
  if(is_separator_appending_a_file(node)){
    isRedirected = true;
    log_message("CommandExecutor.handle_command","Appending to a file");
    append_node_in_file(node);
  }else if(is_separator_redirecting_ouput(node)){
    log_message("CommandExecutor.handle_command","Writing to a file");
    isRedirected = true;
     write_node_in_file(node);
  }
 
  log_message("CommandHandler.handle_command","Check ended, splitting command");
  // Handling command
  char** splitedBySpacesCommand = build_command(node);
  
  // Save file descriptor
  log_message("CommandExecutor.handle_command","Sauvegarde du file descritor..");
  int standardInPutCopy  = dup(1);
  
  log_message("CommandExecutor.handle_command","Création du fichier et ouverture..");
  
  // Empty and open output file
  empty_file(OUTPUT_FILEPATH);
  int fileDescriptorValue  = open(OUTPUT_FILEPATH, O_RDWR | O_CREAT);
  
  log_message("CommandExecutor.handle_command","Handle alias");
  handleAlias(splitedBySpacesCommand);
  log_message("CommandExecutor.handle_command","Start execute command");

  // Checking if it's a custom  command and executing its
  if(!strcmp(splitedBySpacesCommand[0], "setenv"))
  {
    log_message("CommandExecutor.handle_command","Start set env");
    setenv(splitedBySpacesCommand[1],splitedBySpacesCommand[2],0);
  }else if(!strcmp(splitedBySpacesCommand[0], "alias")) {
      if(splitedBySpacesCommand[1] == NULL){
          log_message("CommandExecutor.handle_command","Display alias");
          displayAliases();
      }else{
          log_message("CommandExecutor.handle_command","Update alias");
          updateAliases(splitedBySpacesCommand);    
      }
  }else if(!strcmp(splitedBySpacesCommand[0], "unalias")) {
      removeAlias(splitedBySpacesCommand);
  }else if(!strcmp(splitedBySpacesCommand[0], "pwd")) {
    log_message("CommandExecutor.handle_command","Execution du la commande built in pwd");
    log_message("CommandExecutor.handle_command","Remplacement de la sortie standard par le descripteur du fichier");
    dup2(fileDescriptorValue ,1);
    print_current_directory();
  } else if(!strcmp(splitedBySpacesCommand[0], "cd")) {
    log_message("CommandExecutor.handle_command","Execution du la commande built in cd");
    log_message("CommandExecutor.handle_command","Remplacement de la sortie standard par le descripteur du fichier");
    dup2(fileDescriptorValue ,1);
    change_current_directory(splitedBySpacesCommand[1]);
  } else if(!strcmp(splitedBySpacesCommand[0], "echo")) {
    log_message("CommandExecutor.handle_command","Execution du la commande built in echo");
    log_message("CommandExecutor.handle_command","Remplacement de la sortie standard par le descripteur du fichier");
    dup2(fileDescriptorValue ,1);
    echo(node);
  } else if(!strcmp(splitedBySpacesCommand[0], "exit")) {
    log_message("CommandExecutor.handle_command","Execution du la commande built in exit");
    log_message("CommandExecutor.handle_command","Remplacement de la sortie standard par le descripteur du fichier");
    dup2(fileDescriptorValue ,1);
    exit(EXIT_SUCCESS);
  } else{
    // Executing command from exec
    int forkId = fork();
    checkIfForkSuccessed(forkId);
    if(forkId == 0)
    {
      log_string("CommandExecutor.handle_command","Executing ",node->command);
      log_string("CommandExecutor.handle_command","with token ",splitedBySpacesCommand[0]);
      log_message("CommandExecutor.handle_command","Remplacement de la sortie standard par le descripteur du fichier");
      dup2(fileDescriptorValue ,1);
      execReturnValue = execvp(splitedBySpacesCommand[0],splitedBySpacesCommand);
      exit(1);
      log_message("CommandExecutor.handle_command","Executed");
    }
    int status = 0;
    wait(&status); 
    if(status != 0)
      node->success = false;
    else
      node->success = true;
    useExecvp = true;
  }

  if(!isRedirected){
    checkIfCommandSucceeded(execReturnValue,node->command);
  }
  
  // Get back to normal state
  if(!useExecvp)
  {
    if(errno == 0){
      node->success = true;
    }else{
      node->success = false;
    }
  }

  // Getting back on input
  dup2(standardInPutCopy ,1);
  log_message("CommandExecutor.handle_command","Retour sur le thread normal.");

  //Emptying file
  empty_file(INPUT_FILEPATH);
  log_message("CommandExecutor.handle_command","INPUT FILE EMPTIED.");
  return true;
}

/**
 * Vérifie si la commande command à réussi et gère les erreurs en cas d'échec
 */
void checkIfCommandSucceeded(int execReturnValue, char* command){
  if(execReturnValue == -1){
       dprintf(1,"\nBash: %s: command not found\n",command);
       perror(strerror(errno));
       exit(0);
   }
}

