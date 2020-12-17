CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -fsanitize=address -fsanitize=undefined -fsanitize=leak

# CONFIG
SOURCE=src
OUTPUT=bin

default: build

build: init
	$(CC) $(OPTIONS) -c $(SOURCE)/file.c -o $(OUTPUT)/file.o
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/file.o

init:
	mkdir -p bin

clean:
	rm -rf bin