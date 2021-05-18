CC=clang
OPTIONS=-O0 -W -Wall -Wextra -Wno-unused-command-line-argument -I./include/ -L./lib/ -fsanitize=address -fsanitize=undefined # -fsanitize=leak
LIB=-lSDL2 -lSDL2_image -lpthread -lm

# CONFIG
SOURCE=.
OUTPUT=bin

default: build

build: init
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/graphics.c -o $(OUTPUT)/graphics.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/utilities.c -o $(OUTPUT)/utilities.o
	$(CC) $(DEBUG) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/graphics.o $(OUTPUT)/utilities.o $(LIB)

debug: DEBUG = -DDEBUG
debug: build

init:
	mkdir -p bin

clean:
	rm -rf bin