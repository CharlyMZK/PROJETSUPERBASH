#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "treeUtils.h"
#include "logUtils.h"
#include "stringUtils.h"

/**
 * Vide le fichier path
 */
void empty_file(char* path){
  fclose(fopen(path, "w"));
}

/**
 * supprime le fichier path
 */
void delete_file(char* path){
  remove(path);
}

/**
 * renvoie true si le fichier path est vide
 */
bool is_file_empty(char* path){
  log_message("CommandExecutor.empty_file","Is this file empty ?");
  int size = 0;
  FILE *fptr = fopen(path, "a");
  if (NULL != fptr) {
    fseek (fptr, 0, SEEK_END);
    size = ftell(fptr);

    if (0 == size) {
      log_message("CommandExecutor.empty_file","Yes");
        return true;
    }
  }
  log_message("CommandExecutor.empty_file","No");
 return false;
}

/**
 * Copie le contenu de OUTPUT_FILEPATH dans INPUT_FILEPATH
 */
bool switch_from_file_content_to_file(char* fromFile, char* toFile){
  log_message("CommandExecutor.switchOutputFileContentToInputFile","Preparing input file ..");
  log_string("CommandExecutor.executeCommand","Copying from ",fromFile);
  log_string("CommandExecutor.executeCommand","to ",toFile);
   FILE *fptr1, *fptr2;
    char filename[100], c;
    // Open one file for reading
    fptr1 = fopen(fromFile, "r");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", filename);
        return 0;
    }
 
    // Open another file for writing
    fptr2 = fopen(toFile, "w");
    if (fptr2 == NULL)
    {
        printf("Cannot open file %s \n", filename);
        return 0;
    }
 
    // Read contents from file
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
    
    log_message("CommandExecutor.switchOutputFileContentToInputFile","Content copied from output to input ..");
 
 
    fclose(fptr1);
    fclose(fptr2);
    //delete_file(fromFile);
    //fclose(fopen(OUTPUT_FILEPATH, "w"));
    return 1;
}


void display_file_content(char* path){
    FILE *fptr;
    char  c;
  log_message("CommandExecutor.display_file_content","Displaying file content..");
    // Open file
    fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
 
    // Read contents from file
    c = fgetc(fptr);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fptr);
    }
 
    fclose(fptr);
}