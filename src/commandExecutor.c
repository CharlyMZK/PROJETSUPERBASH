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
#include <stdbool.h>
#include "../include/treeUtils.h"
#include "../include/builtInCommandUtils.h"
#include "../include/commandExecutor.h"
#include "../include/logUtils.h"
#include "../include/stringUtils.h"
#include "../include/fileUtils.h"
#define LSH_RL_BUFSIZE 1024
#define OUTPUT_FILEPATH "./tmpOutputFile"
#define INPUT_FILEPATH "./tmpInputFile"

/**
 * renvoie un tableau de string contenant la commande et ses paramètres 
 */
char** build_command(Node * node)
{
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
     FILE *fptr1;
    // Open one file for reading
    fptr1 = fopen(node->command, "a");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", node->command);
        return 0;
    }
    return switch_from_file_content_to_file(INPUT_FILEPATH,node->command);
}

/**
 * Return true si le séparateur est >
 */
bool is_separator_redirecting_ouput(Node* node){
  return node->inputValue != NULL && (node->inputValue[0] == higher_separator); 
}

/**
 * Execute une commande 
 */
int handle_command(Node* node)
{
  if(is_separator_redirecting_ouput(node)){
     write_node_in_file(node);
  }
  
 char** splitedBySpacesCommand = build_command(node);
 //Sauvegarde du file descripteur
 log_message("CommandExecutor.executeCommand","Sauvegarde du file descritor..");
 int standardInPutCopy  = dup(1);
  
  //Ecriture dans outputfile
  log_message("CommandExecutor.executeCommand","Création du fichier et ouverture..");
  empty_file(OUTPUT_FILEPATH);
  int fileDescriptorValue  = open(OUTPUT_FILEPATH, O_RDWR | O_CREAT);
    //Executing custom command
  if(!strcmp(splitedBySpacesCommand[0], "pwd"))
  {
    log_message("CommandExecutor.executeCommand","Execution du la commande built in pwd");
    log_message("CommandExecutor.executeCommand","Remplacement de la sortie standard par le descripteur du fichier");
    dup2(fileDescriptorValue ,1);
    print_current_directory();
  }
  else if(!strcmp(splitedBySpacesCommand[0], "cd"))
  {
    log_message("CommandExecutor.executeCommand","Execution du la commande built in cd");
    log_message("CommandExecutor.executeCommand","Remplacement de la sortie standard par le descripteur du fichier");
    dup2(fileDescriptorValue ,1);
    change_current_directory(splitedBySpacesCommand[1]);
  }
  else if(!strcmp(splitedBySpacesCommand[0], "echo"))
  {
    log_message("CommandExecutor.executeCommand","Execution du la commande built in echo");
    log_message("CommandExecutor.executeCommand","Remplacement de la sortie standard par le descripteur du fichier");
    dup2(fileDescriptorValue ,1);
    echo(node);
  }
  else if(!strcmp(splitedBySpacesCommand[0], "exit"))
  {
    log_message("CommandExecutor.executeCommand","Execution du la commande built in exit");
    log_message("CommandExecutor.executeCommand","Remplacement de la sortie standard par le descripteur du fichier");
    dup2(fileDescriptorValue ,1);
    exit(EXIT_SUCCESS);
  }
  else
  {
    int forkId = fork();
    if(forkId == 0)
    {
      log_string("CommandExecutor.executeCommand","Executing ",node->command);
      log_string("CommandExecutor.executeCommand","with token ",splitedBySpacesCommand[0]);
      log_message("CommandExecutor.executeCommand","Remplacement de la sortie standard par le descripteur du fichier");
      dup2(fileDescriptorValue ,1);
      execvp(splitedBySpacesCommand[0],splitedBySpacesCommand);
    }
  int status;
  wait(&status); 
  }

   
  // retour a la normale
  dup2(standardInPutCopy ,1);
  log_message("CommandExecutor.executeCommand","Retour sur le thread normal.");
  empty_file(INPUT_FILEPATH);

  display_file_content(OUTPUT_FILEPATH);
  return true;
}