CC=clang
OPTIONS=-O0 -W -Wall -Wextra -fsanitize=address -fsanitize=undefined -fsanitize=leak

# CONFIG
SOURCE=src
OUTPUT= bin

default: build

build: init
	$(CC) $(OPTIONS) -c $(SOURCE)/helpers.c -o $(OUTPUT)/helpers.o
	$(CC) $(OPTIONS) -c $(SOURCE)/tree.c -o $(OUTPUT)/tree.o
	$(CC) $(OPTIONS) -c $(SOURCE)/queue.c -o $(OUTPUT)/queue.o
	$(CC) $(OPTIONS) -c $(SOURCE)/hash.c -o $(OUTPUT)/hash.o
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/queue.o $(OUTPUT)/helpers.o $(OUTPUT)/tree.o $(OUTPUT)/hash.o

init:
	mkdir -p bin

clean:
	rm -r bin