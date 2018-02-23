#include "logUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#define LSH_RL_BUFSIZE 1024
#define OUTPUT_FILEPATH "./tmpOutputFile"
#define INPUT_FILEPATH "./tmpInputFile"


Node* create_tree_from_command(char* command);

bool read_and_exec_tree(Node* treeCommand);

int create_and_execute_tree(char* command);

char** build_command(Node * node);

int handle_command(Node* node);



int execute_command(Node* node);

