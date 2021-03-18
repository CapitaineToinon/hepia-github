all: simple exec

simple: simple.o
	gcc -Wall -s -nostdlib simple.o -o simple

simple.o: simple.asm
	nasm -f elf64 simple.asm

exec: exec.c
	gcc -std=gnu11 -Wall exec.c -o exec

clean:
	rm simple.o simple exec