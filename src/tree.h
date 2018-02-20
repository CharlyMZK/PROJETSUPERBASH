/**
 * @file
 * contient les fonctions relative à l'utilisation de l'arbre
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "logger.h"
#define MAX(X,Y) ((X)>(Y)?(X):(Y))
#define pipe_separator '|'
#define and_separator '&'

/**
 * @brief noeud de l'arbre
 * 
 * Noeud de l'arbre
 */
typedef struct sNode
{
	char* command; /** chaine de caractère contenant la commande*/
	bool success; /** boolean indiquant le succés ou l'echec de la sous commande*/
	char* result; /** chaine de caractère contenant le résultat de la commande */
	char* separator; /** chaine de caractère contenant le séparateur */
	char* inputValue; /** chaine de caractère contenant le la valeur d'entré de la commande */
	struct sNode* leftChild; /** fils droit*/
	struct sNode* rightChild;	/** fils gauche*/
} Node;

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
