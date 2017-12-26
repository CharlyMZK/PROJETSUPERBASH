
all: arbre_bin1

clean:
	rm *.o arbre_bin1


arbre_bin1: arbre_bin1.c 
	gcc arbre_bin1.c -o arbre_bin1

file.o: file.h file.c
	gcc -c file.c

