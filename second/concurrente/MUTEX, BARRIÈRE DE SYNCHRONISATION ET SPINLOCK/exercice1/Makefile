CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -std=gnu11 -fsanitize=address -fsanitize=undefined -fsanitize=leak
LIB=-lpthread -lm

# CONFIG
SOURCE=src
OUTPUT=bin

default: build

build: init
	$(CC) $(OPTIONS) -c $(SOURCE)/barrier.c -o $(OUTPUT)/barrier.o
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/barrier.o $(LIB)

init:
	mkdir -p bin

clean:
	rm -rf bin