#include "builtInCommandUtils.h"
#include "commandExecutor.h"
#include "logUtils.h"
#include "stringUtils.h"
#include "fileUtils.h"
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
 
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
  log_message("CommandExecutor.create_tree_from_command","Creating tree..");
  log_string("CommandExecutor.create_tree_from_command","Command read is",command);
  
  while (index > 0) {
    log_message("CommandExecutor.create_tree_from_command","Row starded");
    log_char_value("CommandExecutor.create_tree_from_command","Letter checked",command[index]);
    if(is_separator(command[index])){
      separator = substr(command,index-1,2);
      truncateLength = lastSeparatorPosition - (index+1);
      truncatedCommand = substr(command,index+1,truncateLength);
      remove_space_at_beginning_and_end(truncatedCommand);
      log_string("CommandExecutor.create_tree_from_command","Separator found",separator);
      log_value("CommandExecutor.create_tree_from_command","Truncating after separator with length",truncateLength);
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
      log_value("CommandExecutor.create_tree_from_command","Last separator position",lastSeparatorPosition);
    }
    index --;
    log_message("CommandExecutor.create_tree_from_command","Row ended");
  }
  log_message("CommandExecutor.create_tree_from_command","While ended,  finishing the three..");

  char *firstCommand = malloc(lastSeparatorPosition + 1);
  
  if(root == NULL){
    log_message("CommandExecutor.create_tree_from_command","Root is null, only one command, creating..");
    remove_space_at_beginning_and_end(command);
    root = create_root(command,NULL,NULL); 
  }else if(actualUsedNode->leftChild == NULL){
    
    log_message("CommandExecutor.create_tree_from_command","Creating last left child");
      log_value("CommandExecutor.create_tree_from_command","lastSeparatorPosition",lastSeparatorPosition);
   
    firstCommand[lastSeparatorPosition] = '\0';
     firstCommand = substr(command,0,lastSeparatorPosition);
    actualUsedNode->leftChild = create_root(firstCommand,NULL,NULL);
  }else if(actualUsedNode->rightChild == NULL){
    log_message("CommandExecutor.create_tree_from_command","Creating last right child");
    log_value("CommandExecutor.create_tree_from_command","lastSeparatorPosition",lastSeparatorPosition);
     firstCommand[lastSeparatorPosition] = '\0';
     firstCommand = substr(command,0,lastSeparatorPosition);
    actualUsedNode->rightChild = create_root(substr(firstCommand,0,lastSeparatorPosition),NULL,NULL);
  }
  
  log_string("CommandExecutor.create_tree_from_command","Root command",root->command);
  log_message("CommandExecutor.create_tree_from_command","Tree created.");
  log_message("CommandExecutor.create_tree_from_command","Printing prefix..");
  log_tree(root);
  printf("\n");
  
  return root;
}



/**
 * Lis un arbre et execute les commandes 
 */
bool read_and_exec_tree(Node* treeCommand){
  log_message("CommandExecutor.read_and_exec_tree","Reading and executing command");
  if(treeCommand->leftChild != NULL){
    
    if(read_and_exec_tree(treeCommand->leftChild)){
      log_message("CommandExecutor.read_and_exec_tree","Leftchild is not null");  
      if(treeCommand->command != NULL){
        log_string("CommandExecutor.read_and_exec_tree","(Command not null ) Executing command",treeCommand->command);
        handle_command(treeCommand);
        if(treeCommand->result != NULL && treeCommand->command != NULL){
          log_string("CommandExecutor.read_and_exec_tree","Command was",treeCommand->command);
          log_string("CommandExecutor.read_and_exec_tree","Result is ",treeCommand->result);
        }
      }else{
        log_message("CommandExecutor.read_and_exec_tree","Command is null, its a separator");
        remove_space_at_beginning_and_end(treeCommand->separator);
        treeCommand->rightChild->inputValue = treeCommand->separator;

        switch_from_file_content_to_file(OUTPUT_FILEPATH,INPUT_FILEPATH);
        return read_and_exec_tree(treeCommand->rightChild);
      }
    }
  }else if(treeCommand->leftChild == NULL && treeCommand->rightChild == NULL){
    log_string("CommandExecutor.read_and_exec_tree","(Both childs Null) Executing command",treeCommand->command);
    bool isExecuted = 0;
    isExecuted = handle_command(treeCommand);
    log_message("CommandExecutor.read_and_exec_tree","This node is at the end of three, command executed.");
    if(treeCommand->result != NULL && treeCommand->command != NULL){
      log_string("CommandExecutor.read_and_exec_tree","Command was",treeCommand->command);
      log_string("CommandExecutor.read_and_exec_tree","Result is ",treeCommand->result);
    }
    return isExecuted;
  }
  return true;
}

/**
 * Analyse la commande passé en paramètre et l'execute
 */
int create_and_execute_tree(char* command){
  log_in_file(command,"./command.txt");
  log_message("CommandExecutor.handle_command","Handling command..");
  Node* treeCommand = create_tree_from_command(command);
  log_message("CommandExecutor.handle_command","Reading and executing tree..");
  read_and_exec_tree(treeCommand);
  log_message("CommandExecutor.handle_command","Cleaning files..");
  return true;
}

char** build_command(Node * node)
{
  char** splitedBySpacesCommand;
  if(!is_file_empty(INPUT_FILEPATH)){
    splitedBySpacesCommand = str_split_and_add_path(node->command,INPUT_FILEPATH);
    log_message("CommandExecutor.executeCommand","Input file isnt empty.");
  }else{
    splitedBySpacesCommand = str_split(node->command, ' ');
    log_string("CommandExecutor.executeCommand","First token : ",splitedBySpacesCommand[0]);
  }
  return splitedBySpacesCommand;
}

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
    echo();
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



