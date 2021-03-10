CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -fsanitize=address -fsanitize=undefined -fsanitize=leak
LIB=-lpthread

# CONFIG
SOURCE=.
OUTPUT=bin

main: init
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/helper.c -o $(OUTPUT)/helper.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/sum.c -o $(OUTPUT)/sum.o
	$(CC) $(DEBUG) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/helper.o $(OUTPUT)/sum.o $(LIB)

debug: DEBUG = -DDEBUG
debug: main

init:
	mkdir -p bin

clean:
	rm -rf bin