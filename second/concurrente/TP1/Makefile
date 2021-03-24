CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -fsanitize=address -fsanitize=undefined # -fsanitize=leak
LIB=-lpthread -lm

# CONFIG
SOURCE=src
OUTPUT=bin

default: build

build: init
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/ker.c -o $(OUTPUT)/ker.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/ppm.c -o $(OUTPUT)/ppm.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/args.c -o $(OUTPUT)/args.o
	$(CC) $(DEBUG) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/ppm.o $(OUTPUT)/ker.o $(OUTPUT)/args.o $(LIB)

debug: DEBUG = -DDEBUG
debug: build

init:
	mkdir -p bin

clean:
	rm -rf bin