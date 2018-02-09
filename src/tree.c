
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tree.h"


Node* create_root(char* command, Node* leftChild, Node* rightChild)
{
	printf("[Log] Creating a node with command : %s\n",command);
	Node* a =malloc(sizeof(Node));
	a->command = command;
	a->leftChild = leftChild;
	a->rightChild = rightChild;
	return a;
}

Node* create_child(char* command)
{
	return create_root(command,NULL,NULL);
}

Node* create_and_return_left_child(Node* a,char* command)
{
	a->leftChild = create_root(command,NULL,NULL);
	return a->leftChild;
}

Node* create_and_return_right_child(Node* a,char* command)
{
	a->rightChild = create_root(command,NULL,NULL);
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


char* getCommand(Node* a)
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
		printf("[ %s ]\n",getCommand(a));
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

void print_infix(Node* a)
{
	if (!is_empty(a))
	{
		print_infix(get_left_child(a));
		printf("%s /",getCommand(a));
		print_infix(get_right_child(a));		
	}
}

void print_postfix(Node* a)
{
	if (!is_empty(a))
	{
		print_postfix(get_left_child(a));
		print_postfix(get_right_child(a));		
		printf("%s /",getCommand(a));
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