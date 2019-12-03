CC=gcc
OPT=-m64

default: build

build:
	$(CC) $(OPT) -c prime.c -o prime.o
	$(CC) $(OPT) -c bezout.c -o bezout.o
	$(CC) $(OPT) -c rsa.c -o rsa.o
	$(CC) $(OPT) main.c -o main prime.o bezout.o rsa.o
	rm *.o

clean: 
	rm *.o main