CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -I/usr/include/ -L./usr/lib/x86_64-linux-gnu # -fsanitize=address -fsanitize=undefined # -fsanitize=leak
LIB=-lSDL2 -lSDL2_image -lpthread -lm

# CONFIG
SOURCE=.
OUTPUT=bin

default: build

build: init
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/game.c -o $(OUTPUT)/game.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/render.c -o $(OUTPUT)/render.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/graphics.c -o $(OUTPUT)/graphics.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/utilities.c -o $(OUTPUT)/utilities.o
	$(CC) $(DEBUG) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/game.o $(OUTPUT)/render.o $(OUTPUT)/graphics.o $(OUTPUT)/utilities.o $(LIB)

debug: DEBUG = -DDEBUG
debug: build

init:
	mkdir -p bin

clean:
	rm -rf bin