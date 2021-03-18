CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -fsanitize=address -fsanitize=undefined -fsanitize=leak
LIB=-lpthread

# CONFIG
SOURCE=.
OUTPUT=bin

default: build

build: init
	$(CC) $(DEBUG) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(LIB)

debug: DEBUG = -DDEBUG
debug: build

init:
	mkdir -p bin

clean:
	rm -rf bin