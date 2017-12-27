
all: superbash

clean:
	rm *.o superbash


superbash: superbash.c 
	gcc superbash.c -o superbash