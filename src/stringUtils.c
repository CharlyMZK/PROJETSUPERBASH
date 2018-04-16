/**
 * @file
 * contient les fonctions relative à l'utilisation des chaines de caractères
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "../include/treeUtils.h"
#include "../include/logUtils.h"
#include "../include/stringUtils.h"

/**
 * retourne un string à partir de la string src à la position pos pour une longueur len 
 */
char *substr(char *src,int pos,int len) { 
  char *dest=NULL;          
  log_message("StringUtils.substr","Substr..");
  log_string("StringUtils.substr","String to sub : ",src);
  log_value("StringUtils.substr","Pos",pos);
  log_value("StringUtils.substr","Length",len);
  if (len>0) {                            
    dest = (char *) malloc(len+1);        
    strncpy(dest,src+pos,len);            
  }                                       
  dest[len] = '\0';
  log_string("StringUtils.substr","String cut",dest);
  return dest;                            
}

/**
 * retourne l'index de la première occurence du caractère charToFind dans la string src
 * ou -1 si le charactère n'est pas présent
 */
int find_index_off_first_occurence_in_string(char *src, char charToFind)
{
  const char *ptr = strchr(src, charToFind);
  if(ptr) 
   return ptr - src;
  else
    return -1;
}


/**
 * Enlève les espaces en début de chaine
 */
void trim_leading(char * str)
{
    int index, i;
    index = 0;
    log_string("StringUtils.trim_leading","String is",str);
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
        log_message("StringUtils.trim_leading","0 put back");
        str[i] = '\0'; //Ferme la string
    }
}

/**
 * Enlève les espaces en fin de chaine
 * 
 */
void trim_last(char * str)
{
    log_string("StringUtils.trim_last","String is",str);
    int index, lastIndex;
    lastIndex = strlen(str);
    index = lastIndex;
    /* Trouve le premier index des derniers espaces */
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index--;
    }
    if(index != lastIndex )
    {
      log_message("StringUtils.trim_last","0 put back");
        str[index] = '\0'; // Make sure that string is NULL terminated
    }
}

/**
 * Enlève les espaces en début et en fin de chaine
 */
void remove_space_at_beginning_and_end(char * string)
{
  log_message("StringUtils.remove_space_at_beginning_and_end","Remove spaces at begining and end");
  trim_leading(string);
  trim_last(string);
}

/**
 * Split la chaine a_str avec le délimiteur a_delim
 */
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    
    return result;
}

/**
 * Split la chaine a_str avec le délimiteur a_delim, et donne la taille du tableau dans size
 */
char** str_split_size_array(char* a_str, const char a_delim, int * size)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    
    *size = count;
    return result;
}

/**
 * Créer un tableau d'arguments en séparant la chaine args pour chaque espace et en ajoutant la chaine path
 */
char** str_split_and_add_path(char * args,char * path)
{
  char a_delim = ' ';
  char** result    = 0;
  size_t count     = 0;
  char* tmp        = args;
  char* last_comma = 0;
  char delim[2];
  delim[0] = a_delim;
  delim[1] = 0;
  /* Count how many elements will be extracted. */
  while (*tmp)
  {
      if (a_delim == *tmp)
      {
          count++;
          last_comma = tmp;
      }
      tmp++;
  }
  /* Add space for trailing token. */
  count += last_comma < (args + strlen(args) - 1);

  /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
  count++;

  result = malloc(sizeof(char*) * count+1);
  if (result)
  {
      size_t idx  = 0;
      char* token = strtok(args, delim);

      while (token)
      {
          assert(idx < count);
          *(result + idx++) = strdup(token);
          token = strtok(0, delim);
      }
      assert(idx == count - 1);
      *(result + idx) = 0;
      result[idx] = path;
  }

  return result;
}

/**
 * Renvois l'index du & si la string command en contiens un à la fin, sinon renvois -1 
 */
int string_contain_and_at_end(char * command)
{
  int index = find_index_off_first_occurence_in_string(command,'&');
  int len = strlen(command);
  if (index + 1 == len && len != 0)
    return index;
  return -1;
}

/**
 * Supprime le caractère de fin si il y a un \n et le remplace par \0
 */
void remove_newline_ch(char *line)
{
    int new_line = strlen(line) -1;
    if (line[new_line] == '\n')
        line[new_line] = '\0';
}

/**
 * Remove a selected char
 */
void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}


/**
 * Renvoie true si la string string est vide et faux sinon
 */
bool isEmptyString(char* string){
     if (string[0] == '\0')
    {
       return true;
    }else{
        return false;
    }
}

/**
 * Renvoie true si la chaine contient un .
 */
bool ifStringContainsDot(char* string){
  int len=strlen(string);
  int i = 0;
  for(i=0;i<len;i++)
    {
        if(string[i]=='.')
        {
        return true;
        
        }
    
    }
  return false;
}

/**
 * Renvoie true si la chaine contient un .
 */
bool ifStringContainsHyphen(char* string){
  int len=strlen(string);
  int i = 0;
  for(i=0;i<len;i++)
    {
        if(string[i]=='-')
        {
        return true;
        
        }
    
    }
  return false;
}