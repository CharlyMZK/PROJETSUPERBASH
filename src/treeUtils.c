/**
 * @file
 * contient les fonctions de l'arbre
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../include/treeUtils.h"
#include "../include/logUtils.h"

/**
 * Return true si le charactère est une séparateur
 */
bool is_separator(char instruction){
	
 log_char_value("Superbash.is_separator","Is this a command ?",instruction); 
 if((instruction == pipe_separator) || (instruction == and_separator) || (instruction == higher_separator) || (instruction == lower_separator)){
   log_message("Superbash.is_separator","Separator found");
   return true;
 }

  return false;
}

/**
 * Créer un noeud 
 * @param command texte de la commande
 * @param leftChild fils gauche
 * @param rightChild fils droit
 */
Node* create_root(char* command, Node* leftChild, Node* rightChild)
{
	Node* a =malloc(sizeof(Node));
	if(is_separator(command[1])){
		log_string("Tree.create_root","Creating a node with separator",command);
		a->separator = command;
	}else{
		log_string("Tree.create_root","Creating a node with command",command);
		a->command = command;	
	}
	
	a->leftChild = leftChild;
	a->rightChild = rightChild;
	return a;
}

/**
 * Créer un noeud
 * @param command texte de la commande
 */
Node* create_child(char* command)
{
	return create_root(command,NULL,NULL);
}

/**
 * Créer et renvois le fils gauche
 */
Node* create_and_return_left_child(Node* a,char* command)
{
	a->leftChild = create_root(command,NULL,NULL);
	return a->leftChild;
}

/**
 * Créer et renvois le fils droit
 */
Node* create_and_return_right_child(Node* a,char* command)
{
	a->rightChild = create_root(command,NULL,NULL);
	return a->rightChild;
}

/**
 * Renvoi le fils gauche
 */
Node* get_left_child(Node* a)
{
	return a->leftChild;
}

/**
 * Renvoi le fils droit
 */
Node* get_right_child(Node* a)
{
	return a->rightChild;
}

/**
 * Renvois la commande du noeud
 */
char* getCommand(Node* a)
{
	return a->command;
}

/**
 * Renvois true sir le noeud a est vide
 */ 
int is_empty(Node* a)
{
	return a == NULL;
}

/**
 * Renvois true si le noeud a est une feuille (si il n'a pas de noeud à droite et à gauche)
 */
int is_child(Node* a)
{
	return (a->leftChild==NULL)&&(a->rightChild==NULL);
}

/**
 * Renvois la hauteur du noeud a, c'est à dire le maximum de la profondeur du fils droit et du fils gauche
 */
int height(Node* a)
{
	if (is_empty(a))
		return 0;
	return 1+ MAX(height(get_left_child(a)),height(get_right_child(a)));
}

/**
 * Renvois la taille du noeud, c'est à dire la somme du nombre de noeud à droite et à gauche
 */
int size(Node* a)
{
	if (is_empty(a))
		return 0;
	return 1+ size(get_left_child(a)) + size(get_right_child(a));
}

/**
 * Affiche le noeud de manière préfixer
 * @param spaceCounter le nombre d'espace à utiliser pour l'indentation du noeud
 */
void print_prefix(int spacesCounter, Node* a)
{	
	if (!is_empty(a))
	{
		int i;
		int j;
		int k;
		for(i = 0; i < spacesCounter ; i++){
			printf(" ");
		}
		if(getCommand(a) != NULL)
			printf("[ %s ]                    Command\n",getCommand(a));
		else
			printf("[ %s ]                    Separator\n",a->separator);
		printf("\n");
		spacesCounter ++ ;
		for(j = 0; j <= spacesCounter ; j++){
			printf(" ");
		}
		printf("[<]");
		print_prefix(spacesCounter,get_left_child(a));
		printf("\n");
		for(k = 0; k <= spacesCounter ; k++){
			printf(" ");
		}
		printf("[>]");
		print_prefix(spacesCounter,get_right_child(a));		
		printf("\n");
	}

}

/**
 * Affiche le noeud de manière infixer
 */
void print_infix(Node* a)
{
	if (!is_empty(a))
	{
		print_infix(get_left_child(a));
		printf("%s /",getCommand(a));
		print_infix(get_right_child(a));		
	}
}

/**
 * Affiche le noeud de manière postfixer
 */
void print_postfix(Node* a)
{
	if (!is_empty(a))
	{
		print_postfix(get_left_child(a));
		print_postfix(get_right_child(a));		
		printf("%s /",getCommand(a));
	}
}

/**
 * Libère la mémoire du noeud a
 */
void clean(Node* a)
{
	if  (!is_empty(a))
	{
		clean(get_left_child(a));
		clean(get_right_child(a));
		free(a);
	}
}


void rec_save_dot(Node* a, FILE* f)
{
	printf("Trying to save dot\n");
	Node *g = get_left_child(a); 
	printf("Left child get\n");
	if (!is_empty(g))	
	{
		printf("[Entered in left]\n");
		printf("[Adding in tree pdf] %s -> %s \n",getCommand(a),getCommand(g));
		fprintf(f,"%s -> %s \n",getCommand(a),getCommand(g));
		rec_save_dot(g,f);
	}
	Node *d = get_right_child(a); 
	printf("Right child get\n");
	if (!is_empty(d))
	{
		printf("[Entered in right]\n");
		printf("[Adding in tree pdf]  %s -> %s \n",getCommand(a),getCommand(d));
		fprintf(f,"%s -> %s \n",getCommand(a),getCommand(d));
		rec_save_dot(d,f);
	}
	printf("Trt ended\n");

}

/**
 * Sauvegarde le noeud dans le fichier .dot
 */
void save_dot(Node* a, char* nom)
{
	// buffer pour chaine de car
	char* buffer = malloc(512);

	// nom du fichier .dot
	sprintf(buffer,"%s.dot",nom);
	printf("%s \n",buffer);

	// ouvre le fichier
	FILE* f = fopen(buffer,"w");

	// ecrit dans le fichier
	printf("[PDF] Trying to create a file.. \n");
	if (!is_empty(a))
	{
		printf("[PDF] node not empty !  \n");
		fprintf(f,"Digraph {\n");
		printf("[Diagraph]\n");
		rec_save_dot(a,f);
		fprintf(f,"}\n");
	}
	fclose(f);
	
	// commande dans la chaine buffer
	sprintf(buffer, "dot -Tpdf -o %s.pdf %s.dot",nom,nom);

	//  execution de la commande
	system(buffer);

	// commande dans la chaine buffer
	//sprintf(buffer, "evince %s.pdf",nom);

	//  execution de la commande
	//system(buffer);

	free(buffer);
}