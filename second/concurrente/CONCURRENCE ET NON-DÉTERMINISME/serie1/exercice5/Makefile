CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -fsanitize=address -fsanitize=undefined -fsanitize=leak
LIB=-lpthread

# CONFIG
SOURCE=.
OUTPUT=bin

default: build

build: init
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(LIB)

init:
	mkdir -p bin

clean:
	rm -rf bin