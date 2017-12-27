
all: superbash

clean:
	rm *.o superbash

superbash: superbash.o
	gcc superbash.o -o superbash

superbash.o: superbash.h superbash.c 
	gcc -c superbash.c 
