#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "treeUtils.h"
#include "stringUtils.h"
#include "../include/aliasUtils.h"
#include "../include/logUtils.h"
#include "../include/fileUtils.h"

char* getCommandName(char* handledAlias, int aliasNameIndex,int commandNameSize){
    char* aliasRelatedCommand = malloc(sizeof(char)*(commandNameSize-2));
    char currentCharGet = '\0';
    int commandNameIndex = aliasNameIndex;
    int i = 0;
    log_message("CommandExecutor.executeCommand","Getting command");
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


void displayAliases(){
    char c[1000];
    FILE *fptr;
    int splitAliasesIndex = 0;
    char* handledAlias;
    char* aliasName;
    char* aliasRelatedCommand;
    int aliasNameSize = 0;
    int aliasNameIndex = 0;
    int commandNameSize = 0;
    int splitAliasesSize = 0;
    char** splitAliases;
    char** split;
    char* commandToExecute;
    
    if ((fptr = fopen("alias.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }

    //reads text until newline 
    fscanf(fptr,"%[^\n]", c);
    splitAliases = str_split(c,'|');
  
    while(splitAliases[splitAliasesSize] != NULL){
      splitAliasesSize++;
    }
  
    if(splitAliasesSize > 0){
        log_message("CommandExecutor.modifyCommandDependingOnAliasDefined","Traitement des alias");

        while(splitAliases[splitAliasesIndex] != NULL){
            log_string("CommandExecutor.modifyCommandDependingOnAliasDefined","Alias traité",splitAliases[splitAliasesIndex]);
            handledAlias = splitAliases[splitAliasesIndex];
            aliasNameSize = getAliasNameSize(handledAlias);
            aliasName = getAliasName(handledAlias,aliasNameSize);
            aliasNameIndex = aliasNameSize;
            commandNameSize = getCommandNameSize(handledAlias, aliasNameIndex);
            aliasRelatedCommand = getCommandName(handledAlias, aliasNameIndex, commandNameSize);
            splitAliasesIndex++;
            log_string("CommandExecutor.modifyCommandDependingOnAliasDefined","Alias name",aliasName);
            printf("\nalias %s='%s'",aliasName,aliasRelatedCommand);
        }
        
        printf("\n");
        split = str_split(c,'=');
        commandToExecute = split[1];
        removeChar(commandToExecute,'\'');
        removeChar(commandToExecute,'|');
  }
}

void updateAliases(char** splitedBySpacesCommand){
    log_message("CommandExecutor.updateAliases","Execution du la commande built in alias");
    log_message("CommandExecutor.updateAliases","Remplacement de la sortie standard par le descripteur du fichier");
    // Opening update aliases files
    FILE *fptr = fopen("alias.txt", "ab+");
    FILE *aliasTmpFilePointer = fopen("aliasTmp.txt", "w");
    char c[1000];
    log_value("CommandExecutor.updateAliases","Taille de c :",strlen(c));
  
    // Init result vars
    int count = 0;
    char* result;
    bool isAliasAdded = false;
   
    if(strlen(c) > 0){
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
      char* enteredAliasName;
      int enteredCommandNameSize = 0;
      char* enteredAliasRelatedCommand;
      bool isSameAlias = false;
      bool isSameCommand = false;
      
 
     
    
      // Getting entered alias informations 
      handledAlias = splitedBySpacesCommand[1];
      enteredAliasNameSize = getAliasNameSize(handledAlias);
      enteredAliasName = getAliasName(handledAlias,enteredAliasNameSize);
      enteredCommandNameSize = getCommandNameSize(handledAlias,enteredAliasNameSize);
      enteredAliasRelatedCommand = getCommandName(handledAlias,enteredAliasNameSize,enteredCommandNameSize);
      log_string("CommandExecutor.updateAliases","[Entered] Alias traité",handledAlias);
      log_string("CommandExecutor.updateAliases","Alias name ( entered ) ",enteredAliasName);
      log_string("CommandExecutor.updateAliases","Command name ( entered ) ",enteredAliasRelatedCommand);
       
      // Get split aliases size
      while(splitAliases[splitAliasesSize] != NULL){
          splitAliasesSize++;
      }
      
      if(splitAliasesSize > 0){
        log_message("CommandExecutor.updateAliases","Traitement des alias");
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
           
            log_string("CommandExecutor.updateAliases","Alias traité",splitAliases[splitAliasesIndex]);
            log_string("CommandExecutor.updateAliases","Alias name",aliasName);
            log_string("CommandExecutor.updateAliases","Alias name entré",enteredAliasName);
           
            // Comparing with entered alias if name or command is the same
            if(!strcmp(enteredAliasName,aliasName)){
                log_message("CommandExecutor.updateAliases","Same alias");
                isSameAlias = true;
            }else{
                log_message("CommandExecutor.updateAliases","Not same alias");
                isSameAlias = false;
            }
            
            if(!strcmp(enteredAliasRelatedCommand,aliasRelatedCommand)){
                log_message("CommandExecutor.updateAliases","Same command");
                isSameCommand = true;
            }else{
                log_message("CommandExecutor.updateAliases","Not same command");
                isSameCommand = false;
            }
            
            if(isSameAlias && !isSameCommand){
                count = strlen(splitedBySpacesCommand[1]);
                result = malloc(sizeof(char*) * count+2);
                result =splitedBySpacesCommand[1];
                isAliasAdded = true;
                log_string("CommandExecutor.updateAliases","(Alias name trouvé mais commande différente) J'écris dans le fichier",result);
            }else{
                count = strlen(handledAlias);
                result = malloc(sizeof(char*) * count+2);
                result = handledAlias;
                log_string("CommandExecutor.updateAliases","(Pas d'alias trouvé) J'écris dans le fichier",result);
            }
            result[count] = '|';
            result[count+1] = '\0';
            fputs(result, aliasTmpFilePointer);
            
        }
      }
  
    }
    if(!isAliasAdded){
        count = strlen(splitedBySpacesCommand[1]);
        result = malloc(sizeof(char*) * count+2);
        result =splitedBySpacesCommand[1];
        fputs(result, aliasTmpFilePointer);
    }
    
    fclose(fptr);
    fclose(aliasTmpFilePointer);
    
    switch_from_file_content_to_file("aliasTmp.txt","alias.txt");
    empty_file("aliasTmp.txt");

}


void handleAlias(char** splitedBySpacesCommand){
     char c[1000];
    FILE *fptr;
  if(!fempty("alias.txt")){
    // Gestion des alias
    
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

void removeAlias(char** splitedBySpacesCommand){
    
    // Init split actuel aliases vars
    char** splitAliases;
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
    char* enteredAliasName;
    int enteredCommandNameSize = 0;
    char* enteredAliasRelatedCommand;
    bool isSameAlias = false;
    bool isSameCommand = false; 
    
    
    // Opening update aliases files
    FILE *fptr = fopen("alias.txt", "ab+");
    FILE *aliasTmpFilePointer = fopen("aliasTmp.txt", "w");
    char c[1000];
    log_value("CommandExecutor.removeAliase","Taille de c :",strlen(c));
  
    // Init result vars
    int count = 0;
    char* result;
    bool isAliasAdded = false;
   
    log_message("CommandExecutor.removeAliase","Execution du la commande built in alias");
    log_message("CommandExecutor.removeAliase","Remplacement de la sortie standard par le descripteur du fichier");
    
    if(strlen(c) > 0){
      fscanf(fptr,"%[^\n]", c);
  
      // Init split actuel aliases vars
      splitAliases = str_split(c,'|');
      splitAliasesIndex = 0;
      handledAlias;
      aliasName;
      aliasRelatedCommand;
      aliasNameSize = 0;
      aliasNameIndex = 0;
      commandNameSize = 0;
      splitAliasesSize = 0;
      
      // Init split entered alias vars
      enteredAliasNameSize = 0;
      enteredAliasName;
      enteredCommandNameSize = 0;
      enteredAliasRelatedCommand;
      isSameAlias = false;
      isSameCommand = false; 
    
      // Getting entered alias informations 
      handledAlias = splitedBySpacesCommand[1];
      enteredAliasNameSize = getAliasNameSize(handledAlias);
      enteredAliasName = getAliasName(handledAlias,enteredAliasNameSize);
      enteredCommandNameSize = getCommandNameSize(handledAlias,enteredAliasNameSize);
      enteredAliasRelatedCommand = getCommandName(handledAlias,enteredAliasNameSize,enteredCommandNameSize);
      log_string("CommandExecutor.removeAliase","[Entered] Alias traité",handledAlias);
      log_string("CommandExecutor.removeAliase","Alias name ( entered ) ",enteredAliasName);
      log_string("CommandExecutor.removeAliase","Command name ( entered ) ",enteredAliasRelatedCommand);
       
      // Get split aliases size
      while(splitAliases[splitAliasesSize] != NULL){
          splitAliasesSize++;
      }
      
      if(splitAliasesSize > 0){
        log_message("CommandExecutor.removeAliase","Traitement des alias");
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
           
            log_string("CommandExecutor.removeAliase","Alias traité",splitAliases[splitAliasesIndex]);
            log_string("CommandExecutor.removeAliase","Alias name",aliasName);
            log_string("CommandExecutor.removeAliase","Alias name entré",enteredAliasName);
           
            // Comparing with entered alias if name or command is the same
            if(!strcmp(enteredAliasName,aliasName)){
                log_message("CommandExecutor.removeAliase","Same alias");
                isSameAlias = true;
            }else{
                log_message("CommandExecutor.removeAliase","Not same alias");
                isSameAlias = false;
            }

               if(!isSameAlias){
                printf("TEST");
                printf("1");
                count = strlen(handledAlias);
                printf("2");
                result = malloc(sizeof(char*) * count+2);
                printf("3");
                result = handledAlias;
                log_string("CommandExecutor.removeAliase","(Pas d'alias trouvé) J'écris dans le fichier",result);
                result[count] = '|';
                result[count+1] = '\0';
                fputs(result, aliasTmpFilePointer);
               }
            
        }
      }
  
    }
    
    fclose(fptr);
    fclose(aliasTmpFilePointer);
    
    switch_from_file_content_to_file("aliasTmp.txt","alias.txt");
    empty_file("aliasTmp.txt");

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
      
      
  log_message("CommandExecutor.modifyCommandDependingOnAliasDefined","Traitement des alias");

    while(splitAliases[splitAliasesIndex] != NULL){
      
      log_string("CommandExecutor.modifyCommandDependingOnAliasDefined","Alias traité",splitAliases[splitAliasesIndex]);
      
      handledAlias = splitAliases[splitAliasesIndex];
      aliasNameSize = getAliasNameSize(handledAlias);
      aliasName = getAliasName(handledAlias,aliasNameSize);
       aliasNameIndex = aliasNameSize;
       commandNameSize = getCommandNameSize(handledAlias, aliasNameIndex);
       aliasRelatedCommand = getCommandName(handledAlias, aliasNameIndex, commandNameSize);
       splitAliasesIndex++;
       
       log_string("CommandExecutor.modifyCommandDependingOnAliasDefined","Alias name",aliasName);
       log_string("CommandExecutor.modifyCommandDependingOnAliasDefined","Commande passée",splitedBySpacesCommand[0]);
       if(!strcmp(splitedBySpacesCommand[0],aliasName)){
          log_message("CommandExecutor.modifyCommandDependingOnAliasDefined","Alias trouvé, remplacement de la commande..");
          splitedBySpacesCommand[0] = aliasRelatedCommand;
       }
    }
  
    char** split = str_split(c,'=');
    char* commandToExecute = split[1];
    removeChar(commandToExecute,'\'');
    removeChar(commandToExecute,'|');
   
  }
  
  
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


int getCommandNameSize(char* handledAlias, int indexToStart){
      int commandNameSize = 0;
      int commandNameIndex = indexToStart;
      char currentCharGet = '\0';
      log_message("CommandExecutor.executeCommand","Getting command name size");
      do{
         currentCharGet = handledAlias[commandNameIndex];
         commandNameIndex++;
         commandNameSize ++ ;
       }while(currentCharGet != '\0');
       return commandNameSize;
}