/**
 * @file
 * contient les fonctions relative à l'utilisation de l'arbre
 */
#include <typedef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define MAX(X,Y) ((X)>(Y)?(X):(Y))
#define pipe_separator '|'
#define and_separator '&'
#define higher_separator '>'
#define lower_separator '<'
#define continue_separator ';'

Node* create_root(char* command, Node* leftChild, Node* rightChild);

Node* create_child(char* command);

Node* get_left_child(Node* a);

Node* get_right_child(Node* a);

Node* create_and_return_left_child(Node* a, char* command);

Node* create_and_return_right_child(Node* a, char* command);

char* getCommand(Node* a);

int is_empty(Node* a);

int is_child(Node* a);

int height(Node* a);

int size(Node* a);

void print_prefix(int spaceCounter, Node* a);

void print_infix(Node* a);

void print_postfix(Node* a);

void clean(Node* a);

bool is_separator(char instruction);