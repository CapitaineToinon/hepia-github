CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined -fsanitize=leak

# CONFIG
SOURCE=src
OUTPUT=bin

default: build

build: init
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/jobs.c -o $(OUTPUT)/jobs.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/wsh.c -o $(OUTPUT)/wsh.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/builtins.c -o $(OUTPUT)/builtins.o
	$(CC) $(DEBUG) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/wsh.o $(OUTPUT)/builtins.o $(OUTPUT)/jobs.o

debug: DEBUG = -DDEBUG
debug: build

init:
	mkdir -p bin

clean:
	rm -r bin