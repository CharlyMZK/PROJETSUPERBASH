
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tree.h"


Node* create_root(char* v, Node* leftChild, Node* rightChild)
{
	Node* a =malloc(sizeof(Node));
	a->command = v;
	a->leftChild = leftChild;
	a->rightChild = rightChild;
	return a;
}

Node* create_child(char* v)
{
	return create_root(v,NULL,NULL);
}

Node* create_and_return_left_child(Node* a,char* v)
{
	a->leftChild = create_root(v,NULL,NULL);
	return a->leftChild;
}

Node* create_and_return_right_child(Node* a,char* v)
{
	a->rightChild = create_root(v,NULL,NULL);
	return a->rightChild;
}



Node* get_left_child(Node* a)
{
	return a->leftChild;
}


Node* get_right_child(Node* a)
{
	return a->rightChild;
}


char* root(Node* a)
{
	return a->command;
}


int is_empty(Node* a)
{
	return a == NULL;
}


int is_child(Node* a)
{
	return (a->leftChild==NULL)&&(a->rightChild==NULL);
}


int height(Node* a)
{
	if (is_empty(a))
		return 0;
	return 1+ MAX(height(get_left_child(a)),height(get_right_child(a)));
}

int size(Node* a)
{
	if (is_empty(a))
		return 0;
	return 1+ size(get_left_child(a)) + size(get_right_child(a));
}


void print_prefix(Node* a)
{	
	if (!is_empty(a))
	{
		printf("%s /",root(a));
		print_prefix(get_left_child(a));
		print_prefix(get_right_child(a));		
	}
}

void print_infix(Node* a)
{
	if (!is_empty(a))
	{
		print_infix(get_left_child(a));
		printf("%s /",root(a));
		print_infix(get_right_child(a));		
	}
}

void print_postfix(Node* a)
{
	if (!is_empty(a))
	{
		print_postfix(get_left_child(a));
		print_postfix(get_right_child(a));		
		printf("%s /",root(a));
	}
}

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
	Node *g = get_left_child(a); 
	if (!is_empty(g))
	{
		fprintf(f,"%s -> %s \n",root(a),root(g));
		rec_save_dot(g,f);
	}
	Node *d = get_right_child(a); 
	if (!is_empty(d))
	{
		fprintf(f,"%s -> %s \n",root(a),root(d));
		rec_save_dot(d,f);
	}

}

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
	if (!is_empty(a))
	{
		fprintf(f,"Digraph {\n");
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

bool is_separator(char* s){
	return strcmp(s,"||") == 0;
}

void create_tree_from_command(int commandSize, char *commandString[]){
	Node* root = create_child("Root");
	Node* actualUsedNode = root;
	int i;
	for (i=1; i < commandSize; i++)
	{
		printf("Argument %d : %s \n", i+1, commandString[i]);
		if(is_separator(commandString[i])){
			printf("Getting back on root..\n");
			actualUsedNode = root;
		}else{
			printf("Continuing the tree..\n");
			if(get_left_child(actualUsedNode) == NULL){
				printf("Creating a new left child..\n");
				actualUsedNode = create_and_return_left_child(actualUsedNode,commandString[i]);
			}else if(get_right_child(actualUsedNode) == NULL){
				printf("Creating a new right child..\n");
				actualUsedNode = create_and_return_right_child(actualUsedNode,commandString[i]);
			}
		}
	}

	printf("Hauteur %d\n",height(root));
	printf("Taille %d\n",size(root));
	printf ("Prefix: ");
	print_prefix(root);
	printf("\n");
		
	save_dot(root,"arbre1");
}

