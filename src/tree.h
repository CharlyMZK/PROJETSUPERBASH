/**
 * @file
 * contient les fonctions relative à l'utilisation de l'arbre
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX(X,Y) ((X)>(Y)?(X):(Y))


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

/**
 * Créer un noeud 
 * @param command texte de la commande
 * @param leftChild fils gauche
 * @param rightChild fils droit
 */
Node* create_root(char* command, Node* leftChild, Node* rightChild);

/**
 * Créer un noeud
 * @param command texte de la commande
 */
Node* create_child(char* command);


/**
 * Renvoi le fils gauche
 */
Node* get_left_child(Node* a);

/**
 * Renvoi le fils droit
 */
Node* get_right_child(Node* a);

/**
 * Créer et renvois le fils gauche
 */
Node* create_and_return_left_child(Node* a, char* command);

/**
 * Créer et renvois le fils droit
 */
Node* create_and_return_right_child(Node* a, char* command);

/**
 * Renvois la commande du noeud
 */
char* getCommand(Node* a);

/**
 * Renvois true sir le noeud a est vide
 */ 
int is_empty(Node* a);
/**
 * Renvois true si le noeud a est une feuille (si il n'a pas de noeud à droite et à gauche)
 */
int is_child(Node* a);

/**
 * Renvois la hauteur du noeud a, c'est à dire le maximum de la profondeur du fils droit et du fils gauche
 */
int height(Node* a);
/**
 * Renvois la taille du noeud, c'est à dire la somme du nombre de noeud à droite et à gauche
 */
int size(Node* a);

//void print_prefix(Node* a);
/**
 * Affiche le noeud de manière préfixer
 * @param spaceCounter le nombre d'espace à utiliser pour l'indentation du noeud
 */
void print_prefix(int spaceCounter, Node* a);

/**
 * Affiche le noeud de manière infixer
 */
void print_infix(Node* a);

/**
 * Affiche le noeud de manière postfixer
 */
void print_postfix(Node* a);

/**
 * Libère la mémoire du noeud a
 */
void clean(Node* a);
