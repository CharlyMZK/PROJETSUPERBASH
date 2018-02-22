#include "commandExecutor.h"
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
 
/**
 * retourne un string à partir de la string src à la position pos pour une longueur len 
 */
char *substr(char *src,int pos,int len) { 
  char *dest=NULL;          
  log_message("CommandExecutor.substr","Substr..");
  log_string("CommandExecutor.substr","String to sub : ",src);
  log_value("CommandExecutor.substr","Pos",pos);
  log_value("CommandExecutor.substr","Length",len);
  if (len>0) {                            
    dest = (char *) malloc(len+1);        
    strncpy(dest,src+pos,len);            
  }                                       
  log_string("CommandExecutor.substr","String cut",dest);
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
 * Log la commande command à la fin du fichier file
 */
bool log_in_file(char * command, char * file)
{
  FILE * pFile = fopen(file, "a");
  if(pFile == NULL)
  {
    printf("Error opening file, impossible to log command \n");
    return false;
  }
  log_message("Superbash","Saving the command in the log file");
  fprintf(pFile, "%s \n",command);
  fclose(pFile);
  return true;
}

/**
 * Execute les commandes passé par ligne de commande
 */
void bash_loop(void)
{
  char *command;
  bool isRunning = true;
  printf("- Prompt launched, Saltscript only please. -\n");
  do {
    printf("Prompt > ");
    command = read_console_line();
    isRunning = handle_command(command);
    //isRunning = executeCommand(command);
  } while (isRunning);
  free(command);
}

/**
 * Enlève les espaces en début de chaine
 */
void trim_leading(char * str)
{
    int index, i;
    index = 0;
    log_string("CommandExecutor.trim_leading","String is",str);
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
        log_message("CommandExecutor.trim_leading","0 put back");
        str[i] = '\0'; //Ferme la string
    }
}

/**
 * Enlève les espaces en fin de chaine
 * 
 */
void trim_last(char * str)
{
    log_string("CommandExecutor.trim_last","String is",str);
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
      log_message("CommandExecutor.trim_last","0 put back");
        str[index] = '\0'; // Make sure that string is NULL terminated
    }
}

/**
 * Enlève les espaces en début et en fin de chaine
 */
void remove_space_at_beginning_and_end(char * string)
{
  log_message("CommandExecutor.remove_space_at_beginning_and_end","Remove spaces at begining and end");
  trim_leading(string);
  trim_last(string);
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
  print_prefix(0,root);
  printf("\n");
  
  return root;
}

/**
 * Copie le contenu de OUTPUT_FILEPATH dans INPUT_FILEPATH
 */
bool switch_output_file_content_to_input_file(){
  log_message("CommandExecutor.switchOutputFileContentToInputFile","Preparing input file ..");
   FILE *fptr1, *fptr2;
    char filename[100], c;
    // Open one file for reading
    fptr1 = fopen(OUTPUT_FILEPATH, "r");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", filename);
        return 0;
    }
 
    // Open another file for writing
    fptr2 = fopen(INPUT_FILEPATH, "w");
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
    delete_file(OUTPUT_FILEPATH);
    //fclose(fopen(OUTPUT_FILEPATH, "w"));
    return 1;
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
        execute_command(treeCommand);
        if(treeCommand->result != NULL && treeCommand->command != NULL){
          log_string("CommandExecutor.read_and_exec_tree","Command was",treeCommand->command);
          log_string("CommandExecutor.read_and_exec_tree","Result is ",treeCommand->result);
        }
      }else{
        log_message("CommandExecutor.read_and_exec_tree","Command is null, its a separator");
        treeCommand->rightChild->inputValue = treeCommand->leftChild->result;
        switch_output_file_content_to_input_file();
        return read_and_exec_tree(treeCommand->rightChild);
      }
    }
  }else if(treeCommand->leftChild == NULL && treeCommand->rightChild == NULL){
    log_string("CommandExecutor.read_and_exec_tree","(Both childs Null) Executing command",treeCommand->command);
    bool isExecuted = 0;
    isExecuted = execute_command(treeCommand);
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
 * Analyse la commande passé en paramètre et l'execute
 */
int handle_command(char* command){
  log_in_file(command,"./command.txt");
  log_message("CommandExecutor.handle_command","Handling command..");
  Node* treeCommand = create_tree_from_command(command);
  log_message("CommandExecutor.handle_command","Reading and executing tree..");
  read_and_exec_tree(treeCommand);
  log_message("CommandExecutor.handle_command","Cleaning files..");
  return true;
}

/**
 * Execute une commande 
 */
int execute_command(Node* node)
{
 //Sauvegarde du file descripteur
 log_message("CommandExecutor.executeCommand","Sauvegarde du file descritor..");
 int standardInPutCopy  = dup(1);

  char** splitedBySpacesCommand;
  if(!is_file_empty(INPUT_FILEPATH)){
    splitedBySpacesCommand = str_split_and_add_path(node->command,INPUT_FILEPATH);
    log_message("CommandExecutor.executeCommand","Input file isnt empty.");
  }else{
    splitedBySpacesCommand = str_split(node->command, ' ');
    log_string("CommandExecutor.executeCommand","First token : ",splitedBySpacesCommand[0]);
  }
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
  return true;
}

/**
 * Affiche le répertoire courant
 */
void print_current_directory()
{
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL)
   fprintf(stdout, "Current working dir: %s\n", cwd);
}

/**
 * Change le répertoire courant
 */
void change_current_directory(char *path)
{
  printf("changing current directory to %s \n",path);
  chdir(path);
  print_current_directory();
}

/**
 * Affiche ECHO
 * TODO passé des paramètres
 */
void echo()
{
  printf("echo\n");
}

/**
 * 
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

void delete_file(char* path){
  remove(path);
}

void empty_file(char* path){
  fclose(fopen(path, "w"));
}

