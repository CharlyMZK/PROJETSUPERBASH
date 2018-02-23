/**
 * @file
 * contient les fonctions relative à l'écriture de log
 */


#include "logUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define VERBOSE 1

/**
 * Affiche [tag]: message si le mode verbose est activé
 */
void log_message(const char* tag, const char* message) {
   if(VERBOSE == 1){
      printf("[%s]: %s \n", tag, message);
   }
}

/**
 * Affiche [tag]: message (parameter) si le mode verbose est activé
 */ 
void log_value(const char* tag, const char* message, int parameter) {
   if(VERBOSE == 1){
      printf("[%s]: %s ( %d ) \n", tag, message,parameter);
   }
}

/**
 * Affiche [tag]: message (parameter) si le mode verbose est activé
 */ 
void log_string(const char* tag, const char* message,const char* parameter) {
   if(VERBOSE == 1){
      printf("[%s]: %s ( %s ) \n", tag, message,parameter);
   }
}

/**
 * Affiche [tag]: message (parameter) si le mode verbose est activé
 */ 
void log_char_value(const char* tag, const char* message, char parameter) {
   if(VERBOSE == 1){
      printf("[%s]: %s ( %c ) \n", tag, message,parameter);
   }
}

void log_tree(Node * node){
   if(VERBOSE == 1){
      print_prefix(0,node);
   }
}

/**
 * Log la commande command à la fin du fichier file
 */
bool log_in_file(char * string, char * file)
{
  FILE * pFile = fopen(file, "a");
  if(pFile == NULL)
  {
    printf("Error opening file, impossible to log string \n");
    return false;
  }
  log_message("Superbash","Saving the string in the log file");
  fprintf(pFile, "%s \n",string);
  fclose(pFile);
  return true;
}