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
  return node->inputValue != NULL && (node->inputValue[0] == higher_separator); 
}

/**
 *  Return true si le séparateur est >>
 */
bool is_separator_appending_a_file(Node* node){
  return node->inputValue != NULL && (node->inputValue[0] == higher_separator && node->inputValue[1]== higher_separator); 
}

int getAliasNameSize(char* handledAlias){
   char currentCharGet = '\0';
   int aliasNameSize = 0;
      log_message("CommandExecutor.executeCommand","Getting alias size");
       while(currentCharGet != '='){
         currentCharGet = handledAlias[aliasNameSize];
         aliasNameSize++;
       }
       return aliasNameSize;
}

char* getAliasName(char* handledAlias, int aliasNameSize){
  
  
      char* aliasName = malloc(sizeof(char)*aliasNameSize);
      char currentCharGet = '\0';
      int aliasNameIndex = 0;
      log_message("CommandExecutor.executeCommand","Getting alias name");
       do{
         currentCharGet = handledAlias[aliasNameIndex];
         aliasName[aliasNameIndex] = handledAlias[aliasNameIndex];
         aliasNameIndex++;
         
       }while(currentCharGet != '=');
       
       aliasName[aliasNameIndex] = '\0';
       removeChar(aliasName,'=');
       log_string("CommandExecutor.executeCommand","Alias name",aliasName);
       return aliasName;
}

int getCommandNameSize(char* handledAlias, int indexToStart){
    log_message("CommandExecutor.executeCommand","Getting command name size");
      int commandNameSize = 0;
      int commandNameIndex = indexToStart;
      char currentCharGet = '\0';
        do{
         currentCharGet = handledAlias[commandNameIndex];
         commandNameIndex++;
         commandNameSize ++ ;
       }while(currentCharGet != '\0');
       return commandNameSize;
       
}

char* getCommandName(char* handledAlias, int aliasNameIndex,int commandNameSize){
  log_message("CommandExecutor.executeCommand","Getting command");
       char* aliasRelatedCommand = malloc(sizeof(char)*(commandNameSize-2));
       char currentCharGet = '\0';
       int commandNameIndex = aliasNameIndex;
       int i = 0;
        do{
         currentCharGet = handledAlias[commandNameIndex];
         if(currentCharGet != '\''){
          aliasRelatedCommand[i] = currentCharGet;
          i++;
         }
    
         commandNameIndex++;
         
       }while(currentCharGet != '\0');
           aliasRelatedCommand[i] = '\0';
       log_string("CommandExecutor.executeCommand","Command name",aliasRelatedCommand);
       return aliasRelatedCommand;
}

int fempty(char const *fname) 
{
	FILE *fdesc = fopen(fname,"r");
	int ret = 0;
 
	if( fdesc )
	{
		(void)fgetc(fdesc);
		if( feof(fdesc) )
		{
			ret = 1;
		}
		fclose(fdesc);
	}
	return ret;
}

void modifyCommandDependingOnAliasDefined(char* c, char** splitedBySpacesCommand){
    char** splitAliases = str_split(c,'|');
  int splitAliasesIndex = 0;
  char* handledAlias;
  char* aliasName;
  char* aliasRelatedCommand;
  int aliasNameSize = 0;
  int aliasNameIndex = 0;
  int commandNameSize = 0;
  int splitAliasesSize = 0;
  
  while(splitAliases[splitAliasesSize] != NULL){
      splitAliasesSize++;
  }
  
  if(splitAliasesSize > 0){
  log_message("CommandExecutor.executeCommand","Traitement des alias");

    while(splitAliases[splitAliasesIndex] != NULL){
      
      log_string("CommandExecutor.executeCommand","Alias traité",splitAliases[splitAliasesIndex]);
      
      handledAlias = splitAliases[splitAliasesIndex];
      aliasNameSize = getAliasNameSize(handledAlias);
      aliasName = getAliasName(handledAlias,aliasNameSize);
       aliasNameIndex = aliasNameSize;
       commandNameSize = getCommandNameSize(handledAlias, aliasNameIndex);
       aliasRelatedCommand = getCommandName(handledAlias, aliasNameIndex, commandNameSize);
       splitAliasesIndex++;
       
       log_string("CommandExecutor.executeCommand","Alias name",aliasName);
       log_string("CommandExecutor.executeCommand","Commande passée",splitedBySpacesCommand[0]);
       if(!strcmp(splitedBySpacesCommand[0],aliasName)){
          log_message("CommandExecutor.executeCommand","Alias trouvé, remplacement de la commande..");
          splitedBySpacesCommand[0] = aliasRelatedCommand;
       }
    }
  
    char** split = str_split(c,'=');
    char* commandToExecute = split[1];
    removeChar(commandToExecute,'\'');
    removeChar(commandToExecute,'|');
   
  }
  
  
}


void addOrReplaceAlias(char** splitedBySpacesCommand){
    
    log_message("CommandExecutor.executeCommand","Execution du la commande built in alias");
    log_message("CommandExecutor.executeCommand","Remplacement de la sortie standard par le descripteur du fichier");
      FILE *fptr = fopen("alias.txt", "ab+");
    char c[1000];
     // reads text until newline 
  fscanf(fptr,"%[^\n]", c);
    char** splitAliases = str_split(c,'|');
  int splitAliasesIndex = 0;
  char* handledAlias;
  char* aliasName;
  char* aliasRelatedCommand;
  int aliasNameSize = 0;
  int aliasNameIndex = 0;
  int commandNameSize = 0;
  int splitAliasesSize = 0;
  
  int enteredAliasNameSize = 0;
  int enteredAliasNameIndex = 0;
  char* enteredAliasName;
  int enteredCommandNameSize = 0;
  int enteredSplitAliasesSize = 0;
  char* enteredAliasRelatedCommand;
  bool isSameAlias = false;
  bool isSameCommand = false;
  
  
    handledAlias = splitedBySpacesCommand[1];
  log_string("CommandExecutor.executeCommand","[Entered] Alias traité",handledAlias);
  enteredAliasNameSize = getAliasNameSize(handledAlias);
  enteredAliasName = getAliasName(handledAlias,enteredAliasNameSize);
  enteredCommandNameSize = getCommandNameSize(handledAlias,enteredAliasNameSize);
  enteredAliasRelatedCommand = getCommandName(handledAlias,enteredAliasNameSize,enteredCommandNameSize);
  log_value("CommandExecutor.executeCommand","Alias size ( entered ) ",enteredAliasNameSize);
  log_string("CommandExecutor.executeCommand","Alias name ( entered ) ",enteredAliasName);
  log_value("CommandExecutor.executeCommand","Command size ( entered ) ",enteredCommandNameSize);
  log_string("CommandExecutor.executeCommand","Command name ( entered ) ",enteredAliasRelatedCommand);
  
  
  while(splitAliases[splitAliasesSize] != NULL){
      splitAliasesSize++;
  }
  
  if(splitAliasesSize > 0){
  log_message("CommandExecutor.executeCommand","Traitement des alias");

    while(splitAliases[splitAliasesIndex] != NULL){
      
      log_string("CommandExecutor.executeCommand","Alias traité",splitAliases[splitAliasesIndex]);
      
      handledAlias = splitAliases[splitAliasesIndex];
      aliasNameSize = getAliasNameSize(handledAlias);
      aliasName = getAliasName(handledAlias,aliasNameSize);
       aliasNameIndex = aliasNameSize;
       commandNameSize = getCommandNameSize(handledAlias, aliasNameIndex);
       aliasRelatedCommand = getCommandName(handledAlias, aliasNameIndex, commandNameSize);
       splitAliasesIndex++;
       
       log_string("CommandExecutor.executeCommand","Alias name",aliasName);
       log_string("CommandExecutor.executeCommand","Commande passée",splitedBySpacesCommand[0]);
       
       
    }
  
    char** split = str_split(c,'=');
    char* commandToExecute = split[1];
    removeChar(commandToExecute,'\'');
    removeChar(commandToExecute,'|');
    
    
    
     if(!strcmp(enteredAliasName,aliasName)){
           isSameAlias = true;
       }else{
           isSameAlias = false;
       }
       if(strcmp(enteredAliasRelatedCommand,aliasRelatedCommand)){
           isSameCommand = true;
       }else{
           isSameCommand = false;
       }
  if(isSameAlias && !isSameCommand){
      log_message("CommandExecutor.executeCommand","Nom de l'alias egal, mais pas la commande, remplacement..");
      //     fclose(fptr);
           printf("Content : %s",c);
        //fptr = fopen("alias.txt", "w");
        //fputs("test", fptr);
       // fclose(fptr);
  }
  
   
  } 
  
  

  
      
  
  
   if (fptr != NULL)
    {
      int count = strlen(splitedBySpacesCommand[1]);
      char* result = malloc(sizeof(char*) * count+2);
       result = splitedBySpacesCommand[1];
       result[count] = '|';
          result[count+1] = '\0';
       printf(" result : %s",result);
        fputs(result, fptr);
        fclose(fptr);
    }

}



void updateAliases(char** splitedBySpacesCommand){
  log_message("CommandExecutor.executeCommand","Execution du la commande built in alias");
  log_message("CommandExecutor.executeCommand","Remplacement de la sortie standard par le descripteur du fichier");
  // Opening update aliases files
  FILE *fptr = fopen("alias.txt", "ab+");
  FILE *aliasTmpFilePointer = fopen("aliasTmp.txt", "w");
  char c[1000];
  fscanf(fptr,"%[^\n]", c);
  
  // Init split actuel aliases vars
  char** splitAliases = str_split(c,'|');
  int splitAliasesIndex = 0;
  char* handledAlias;
  char* aliasName;
  char* aliasRelatedCommand;
  int aliasNameSize = 0;
  int aliasNameIndex = 0;
  int commandNameSize = 0;
  int splitAliasesSize = 0;
  
  // Init split entered alias vars
  int enteredAliasNameSize = 0;
  int enteredAliasNameIndex = 0;
  char* enteredAliasName;
  int enteredCommandNameSize = 0;
  int enteredSplitAliasesSize = 0;
  char* enteredAliasRelatedCommand;
  bool isSameAlias = false;
  bool isSameCommand = false;

  // Getting entered alias informations 
  handledAlias = splitedBySpacesCommand[1];
  enteredAliasNameSize = getAliasNameSize(handledAlias);
  enteredAliasName = getAliasName(handledAlias,enteredAliasNameSize);
  enteredCommandNameSize = getCommandNameSize(handledAlias,enteredAliasNameSize);
  enteredAliasRelatedCommand = getCommandName(handledAlias,enteredAliasNameSize,enteredCommandNameSize);
  log_string("CommandExecutor.executeCommand","[Entered] Alias traité",handledAlias);
  log_string("CommandExecutor.executeCommand","Alias name ( entered ) ",enteredAliasName);
  log_string("CommandExecutor.executeCommand","Command name ( entered ) ",enteredAliasRelatedCommand);
   
  // Get split aliases size
  while(splitAliases[splitAliasesSize] != NULL){
      splitAliasesSize++;
  }
  
  if(splitAliasesSize > 0){
    log_message("CommandExecutor.executeCommand","Traitement des alias");
    // Iterating on existing file aliases
    while(splitAliases[splitAliasesIndex] != NULL){
      
        // Get alias info
        handledAlias = splitAliases[splitAliasesIndex];
        aliasNameSize = getAliasNameSize(handledAlias);
        aliasName = getAliasName(handledAlias,aliasNameSize);
        aliasNameIndex = aliasNameSize;
        commandNameSize = getCommandNameSize(handledAlias, aliasNameIndex);
        aliasRelatedCommand = getCommandName(handledAlias, aliasNameIndex, commandNameSize);
        splitAliasesIndex++;
       
        log_string("CommandExecutor.executeCommand","Alias traité",splitAliases[splitAliasesIndex]);
        log_string("CommandExecutor.executeCommand","Alias name",aliasName);
        log_string("CommandExecutor.executeCommand","Alias name entré",enteredAliasName);
       
        // Comparing with entered alias if name or command is the same
        if(!strcmp(enteredAliasName,aliasName)){
            log_message("CommandExecutor.executeCommand","Same alias");
            isSameAlias = true;
        }else{
            log_message("CommandExecutor.executeCommand","Not same alias");
            isSameAlias = false;
        }
        
        if(!strcmp(enteredAliasRelatedCommand,aliasRelatedCommand)){
            log_message("CommandExecutor.executeCommand","Same command");
            isSameCommand = true;
        }else{
            log_message("CommandExecutor.executeCommand","Not same command");
            isSameCommand = false;
        }
        
        if(isSameAlias && !isSameCommand){
            int count = strlen(splitedBySpacesCommand[1]);
            char* result = malloc(sizeof(char*) * count+2);
            result =splitedBySpacesCommand[1];
            result[count] = '|';
            result[count+1] = '\0';
            fputs(result, aliasTmpFilePointer);
            log_string("CommandExecutor.executeCommand","(Alias name trouvé mais commande différente) J'écris dans le fichier",result);
        }else{
            int count = strlen(handledAlias);
            char* result = malloc(sizeof(char*) * count+2);
            result = handledAlias;
            result[count] = '|';
            result[count+1] = '\0';
            fputs(result, aliasTmpFilePointer);
            log_string("CommandExecutor.executeCommand","(Pas d'alias trouvé) J'écris dans le fichier",result);
        }
    }
    fclose(fptr);
    fclose(aliasTmpFilePointer);
    
    switch_from_file_content_to_file("aliasTmp.txt","alias.txt");
    empty_file("aliasTmp.txt");
 }
}

void handleAlias(char** splitedBySpacesCommand){
      if(!fempty("alias.txt")){
  // Gestion des alias
  char c[1000];
  FILE *fptr;
  
  if ((fptr = fopen("alias.txt", "r")) == NULL)
  {
    printf("Error! opening file");
    // Program exits if file pointer returns NULL.
    exit(1);         
  }

  // reads text until newline 
  fscanf(fptr,"%[^\n]", c);


    modifyCommandDependingOnAliasDefined(c,splitedBySpacesCommand);
   fclose(fptr);
  }
  
}


/**ls
 * Execute une commande 
 */
int handle_command(Node* node)
{
  // If the node got < or << we are redirecting the output instead of executing command
  if(is_separator_appending_a_file(node)){
    log_message("CommandExecutor.handle_command","Appending to a file");
    append_node_in_file(node);
  }else if(is_separator_redirecting_ouput(node)){
    log_message("CommandExecutor.handle_command","Writing to a file");
     write_node_in_file(node);
  }
  
 // Handling command
 char** splitedBySpacesCommand = build_command(node);
 
 //Sauvegarde du file descripteur
 log_message("CommandExecutor.executeCommand","Sauvegarde du file descritor..");
 int standardInPutCopy  = dup(1);
  
  //Début de l'écriture dans outputfile
  log_message("CommandExecutor.executeCommand","Création du fichier et ouverture..");
  
  printf("FEMPTY ? %d",fempty("alias.txt"));
  
  empty_file(OUTPUT_FILEPATH);
  int fileDescriptorValue  = open(OUTPUT_FILEPATH, O_RDWR | O_CREAT);
  

  handleAlias(splitedBySpacesCommand);


  //Executing custom command
  if(!strcmp(splitedBySpacesCommand[0], "alias"))
  {
      updateAliases(splitedBySpacesCommand);
    /*log_message("CommandExecutor.executeCommand","Execution du la commande built in alias");
    log_message("CommandExecutor.executeCommand","Remplacement de la sortie standard par le descripteur du fichier");
      FILE *fp = fopen("alias.txt", "ab+");
    if (fp != NULL)
    {
      int count = strlen(splitedBySpacesCommand[1]);
      char* result = malloc(sizeof(char*) * count+2);
       result = splitedBySpacesCommand[1];
       result[count] = '|';
          result[count+1] = '\0';
       printf(" result : %s",result);
        fputs(result, fp);
        fclose(fp);
    }*/
  }
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
    // Executing command from exec
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
  
  // Get back to normal state
  dup2(standardInPutCopy ,1);
  log_message("CommandExecutor.executeCommand","Retour sur le thread normal.");
  empty_file(INPUT_FILEPATH);
  return true;
}

