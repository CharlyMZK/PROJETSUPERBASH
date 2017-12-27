
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX(X,Y) ((X)>(Y)?(X):(Y))

typedef struct sNode
{
	char* command;
	bool success;
	char* result;
	char* separator;
	char* inputValue;
	struct sNode* leftChild;
	struct sNode* rightChild;	
} Node;

Node* create_root(char* v, Node* leftChild, Node* rightChild);

Node* create_child(char* v);

Node* create_left_child(Node* a);

Node* create_right_child(Node* a);

char* root(Node* a);

int is_empty(Node* a);

int is_child(Node* a);

int height(Node* a);

int size(Node* a);

void print_prefix(Node* a);

void print_infix(Node* a);

void print_postfix(Node* a);

void clean(Node* a);
