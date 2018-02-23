#include "logUtils.h"
#include "stringUtils.h"
#include <stdbool.h>

Node* create_tree_from_command(char* command);

bool read_and_exec_tree(Node* treeCommand);

int create_and_execute_tree(char* command);