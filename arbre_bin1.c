
#include <stdio.h>
#include <stdlib.h>

#define MAX(X,Y) ((X)>(Y)?(X):(Y))

typedef int T;

typedef struct sArbre
{
	T label;
	struct sArbre* fg;
	struct sArbre* fd;	
} Arbre;

Arbre* enracine(T v, Arbre* fg, Arbre* fd)
{
	Arbre* a =malloc(sizeof(Arbre));
	a->label = v;
	a->fg = fg;
	a->fd = fd;
	return a;
}

Arbre* creer_feuille(T v)
{
	return enracine(v,NULL,NULL);
}

Arbre* fils_gauche(Arbre* a)
{
	return a->fg;
}


Arbre* fils_droit(Arbre* a)
{
	return a->fd;
}


T racine(Arbre* a)
{
	return a->label;
}


int est_vide(Arbre* a)
{
	return a == NULL;
}


int est_feuille(Arbre* a)
{
	return (a->fg==NULL)&&(a->fd==NULL);
}


int hauteur(Arbre* a)
{
	if (est_vide(a))
		return 0;
	return 1+ MAX(hauteur(fils_gauche(a)),hauteur(fils_droit(a)));
}

int taille(Arbre* a)
{
	if (est_vide(a))
		return 0;
	return 1+ taille(fils_gauche(a)) + taille(fils_droit(a));
}


void print_prefix(Arbre* a)
{	
	if (!est_vide(a))
	{
		printf("%d /",racine(a));
		print_prefix(fils_gauche(a));
		print_prefix(fils_droit(a));		
	}
}

void print_infix(Arbre* a)
{
	if (!est_vide(a))
	{
		print_infix(fils_gauche(a));
		printf("%d /",racine(a));
		print_infix(fils_droit(a));		
	}
}

void print_postfix(Arbre* a)
{
	if (!est_vide(a))
	{
		print_postfix(fils_gauche(a));
		print_postfix(fils_droit(a));		
		printf("%d /",racine(a));
	}
}


void rec_save_dot(Arbre* a, FILE* f)
{
	Arbre *g = fils_gauche(a); 
	if (!est_vide(g))
	{
		fprintf(f,"%d -> %d \n",racine(a),racine(g));
		rec_save_dot(g,f);
	}
	Arbre *d = fils_droit(a); 
	if (!est_vide(d))
	{
		fprintf(f,"%d -> %d \n",racine(a),racine(d));
		rec_save_dot(d,f);
	}
}


void affiche_dot(Arbre* a, char* nom)
{
	// buffer pour chaine de car
	char* buffer = malloc(512);

	// nom du fichier .dot
	sprintf(buffer,"%s.dot",nom);

	// ouvre le fichier
	FILE* f = fopen(buffer,"w");

	// ecrit dans le fichier
	if (!est_vide(a))
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


void clean(Arbre* a)
{
	if  (!est_vide(a))
	{
		clean(fils_gauche(a));
		clean(fils_droit(a));
		free(a);
	}
}

int main()
{
	Arbre * a = enracine(1,enracine(2,creer_feuille(4),creer_feuille(5)),enracine(3,creer_feuille(6),enracine(7,creer_feuille(8),NULL)));

	printf("Hauteur %d\n",hauteur(a));

	printf("Taille %d\n",taille(a));

	printf ("Prefix: ");
	print_prefix(a);
	printf("\n");

	printf ("Infix: ");
	print_infix(a);
	printf("\n");

	printf ("Postfix: ");
	print_postfix(a);
	printf("\n");

	affiche_dot(a,"arbre1");

	return 0;
}
