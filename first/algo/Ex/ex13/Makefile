CC=gcc
OPTIONS=-g -O0 -W -Wall

# CONFIG
SOURCE=src
OUTPUT= bin

default: build

build: init
	$(CC) $(OPTIONS) -c $(SOURCE)/stack.c -o $(OUTPUT)/stack.o
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/stack.o

init:
	mkdir -p bin

clean:
	rm -r bin