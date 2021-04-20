CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -std=gnu11 -fsanitize=address -fsanitize=undefined -fsanitize=leak
LIB=-lpthread -lm

# CONFIG
SOURCE=src
OUTPUT=bin

default: build

build: init
	$(CC) $(OPTIONS) -c $(SOURCE)/variables.c -o $(OUTPUT)/variables.o
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/variables.o $(LIB)

init:
	mkdir -p bin

clean:
	rm -rf bin