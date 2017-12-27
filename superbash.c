
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

int main()
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

	return 0;
}
