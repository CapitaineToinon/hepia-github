CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -Wshadow -std=gnu11 -lrt -fsanitize=address -fsanitize=undefined -fsanitize=leak
LIB=-lpthread -lm

# CONFIG
SOURCE=src
OUTPUT=bin

default: build

build: init
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(LIB)

init:
	mkdir -p bin

clean:
	rm -rf bin