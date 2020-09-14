CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -fsanitize=address -fsanitize=undefined

# CONFIG
SOURCE=src
OUTPUT= bin

default: build

build: init
	$(CC) $(OPTIONS) -c $(SOURCE)/queue.c -o $(OUTPUT)/queue.o
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/queue.o

init:
	mkdir -p bin

clean:
	rm -r bin