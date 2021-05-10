CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined -fsanitize=leak

# CONFIG
SOURCE=src
OUTPUT=bin

default: build

build: init
	$(CC) $(OPTIONS) -c $(SOURCE)/wsh.c -o $(OUTPUT)/wsh.o
	$(CC) $(OPTIONS) -c $(SOURCE)/process.c -o $(OUTPUT)/process.o
	$(CC) $(OPTIONS) -c $(SOURCE)/builtins.c -o $(OUTPUT)/builtins.o
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/wsh.o $(OUTPUT)/builtins.o $(OUTPUT)/process.o

init:
	mkdir -p bin

clean:
	rm -r bin