#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "tree.h"
#include "logger.h"
#define LSH_RL_BUFSIZE 1024

char *substr(char *src,int pos,int len);

int *find_index_off_first_occurence_in_string(char *src, char charToFind);

char *read_console_line(void);

bool log_in_file(char * command, char * file);

void bash_loop(void);

void trim_leading(char * str);

void trim_last(char * str);

void remove_space_at_beginning_and_end(char * string);

Node* create_tree_from_command(char* command);

bool read_and_exec_tree(Node* treeCommand);

int handle_command(char* command);

int execute_command(Node* node);

void print_current_directory();

void change_current_directory(char *path);

bool is_file_empty(char* fp);

void delete_file(char* path);

bool switchOutputFileContentToInputFile();

int execute_command(Node* node);

void empty_file(char* path);