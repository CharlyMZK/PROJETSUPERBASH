
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "superbash.h"

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

Node* create_left_child(Node* a)
{
	return a->leftChild;
}


Node* create_right_child(Node* a)
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
	return 1+ MAX(height(create_left_child(a)),height(create_right_child(a)));
}

int size(Node* a)
{
	if (is_empty(a))
		return 0;
	return 1+ size(create_left_child(a)) + size(create_right_child(a));
}


void print_prefix(Node* a)
{	
	if (!is_empty(a))
	{
		printf("%s /",root(a));
		print_prefix(create_left_child(a));
		print_prefix(create_right_child(a));		
	}
}

void print_infix(Node* a)
{
	if (!is_empty(a))
	{
		print_infix(create_left_child(a));
		printf("%s /",root(a));
		print_infix(create_right_child(a));		
	}
}

void print_postfix(Node* a)
{
	if (!is_empty(a))
	{
		print_postfix(create_left_child(a));
		print_postfix(create_right_child(a));		
		printf("%s /",root(a));
	}
}

void clean(Node* a)
{
	if  (!is_empty(a))
	{
		clean(create_left_child(a));
		clean(create_right_child(a));
		free(a);
	}
}


void rec_save_dot(Node* a, FILE* f)
{
	Node *g = create_left_child(a); 
	if (!is_empty(g))
	{
		fprintf(f,"%s -> %s \n",root(a),root(g));
		rec_save_dot(g,f);
	}
	Node *d = create_right_child(a); 
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
	sprintf(buffer, "evince %s.pdf",nom);

	//  execution de la commande
	system(buffer);

	free(buffer);
}


int main(int argc, char *argv[])
{
	Node * a = create_root("test",create_root("ad",create_child("adz"),create_child("azd")),create_root("adz",create_child("adaz"),create_root("azd",create_child("adzd"),NULL)));

	printf("Hauteur %d\n",height(a));

	printf("Taille %d\n",size(a));

	printf ("Prefix: ");
	print_prefix(a);
	printf("\n");

	printf ("Infix: ");
	print_infix(a);
	printf("\n");

	printf ("Postfix: ");
	print_postfix(a);
	printf("\n");

	

	Node* b = create_root("Root");
	int i;
	
	for (i=0; i < argc; i++)
	{
		printf("Argument %d : %s \n", i+1, argv[i]);
		create_child(argv[i])
	}

	save_dot(b,"arbre1");

	return 0;
}
