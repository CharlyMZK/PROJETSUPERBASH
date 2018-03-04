/**
 * @file
 * contient les fonctions relative à l'écriture et la lecture de fichiers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../include/treeUtils.h"
#include "../include/logUtils.h"
#include "../include/stringUtils.h"

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
  log_message("FileUtils.empty_file","Is this file empty ?");
  int size = 0;
  FILE *fptr = fopen(path, "a");
  if (NULL != fptr) {
    fseek (fptr, 0, SEEK_END);
    size = ftell(fptr);

    if (0 == size) {
      log_message("FileUtils.empty_file","Yes");
        return true;
    }
  }
  log_message("FileUtils.empty_file","No");
 return false;
}

/**
 * Copie le contenu de OUTPUT_FILEPATH dans INPUT_FILEPATH
 */
bool switch_from_file_content_to_file(char* fromFile, char* toFile){
    log_message("FileUtils.switch_from_file_content_to_file","Preparing input file ..");
    log_string("FileUtils.switch_from_file_content_to_file","Copying from ",fromFile);
    log_string("FileUtils.switch_from_file_content_to_file","to ",toFile);
    FILE *fptr1, *fptr2;
    char c;
    // Open one file for reading
    fptr1 = fopen(fromFile, "r");
    if (fptr1 == NULL)
    {
        log_string("FileUtils.switch_from_file_content_to_file","Cannot open file ",fromFile);
        return 0;
    }
 
    // Open another file for writing
    fptr2 = fopen(toFile, "w");
    if (fptr2 == NULL)
    {
        log_string("FileUtils.switch_from_file_content_to_file","Cannot open file ",toFile);
        return 0;
    }   
 
    // Read contents from file
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
    log_message("FileUtils.switch_from_file_content_to_file","Content copied from output to input ..");
    fclose(fptr1);
    fclose(fptr2);
    return 1;
}

bool append_from_file_content_to_file(char* fromFile, char* toFile){
   log_message("FileUtils.append_from_file_content_to_file","Preparing input file ..");
   log_string("FileUtils.append_from_file_content_to_file","Copying from ",fromFile);
   log_string("FileUtils.append_from_file_content_to_file","to ",toFile);
   FILE *fptr1, *fptr2;
    char c;
    // Open one file for reading
    fptr1 = fopen(fromFile, "r");
    if (fptr1 == NULL)
    {
        log_string("FileUtils.switch_from_file_content_to_file","Cannot open file ",fromFile);
        return 0;
    }
 
    // Open another file for writing
    fptr2 = fopen(toFile, "a+");
    if (fptr2 == NULL)
    {
        log_string("FileUtils.switch_from_file_content_to_file","Cannot open file ",toFile);
        return 0;
    }
 
    // Read contents from file
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
    log_message("FileUtils.append_from_file_content_to_file","Content copied from output to input ..");
    fclose(fptr1);
    fclose(fptr2);
    return 1;
}

/**
 * Affiche le contenu du fichier path
 */
void display_file_content(char* path){
    FILE *fptr;
    char  c;
  log_message("FileUtils.display_file_content","Displaying file content..");
    // Open file
    fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        log_message("FileUtils.append_from_file_content_to_file","Cannot open file");
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