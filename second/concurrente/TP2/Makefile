# GCC=clang -O2 -Wall -std=gnu99 -g
# LIBS=-I./include/ -L./lib/ -lSDL2 -lSDL2_image -lpthread

# BIN=main
# SRCS=$(wildcard *.c)
# OBJS=$(SRCS:.c=.o)
# HEADERS=$(wildcard *.h)

# $(BIN): $(OBJS)
# 	$(GCC) $^ -o $@ $(LIBS)

# run: $(BIN)
# 	./$< 

# %.o: %.c $(HEADERS)
# 	$(GCC) $< -c

# clean:
# 	rm -f $(BIN) $(OBJS)

CC=clang
OPTIONS=-O0 -W -Wall -Wextra -Wno-unused-command-line-argument -I./include/ -L./lib/ -fsanitize=address -fsanitize=undefined # -fsanitize=leak
LIB=-lSDL2 -lSDL2_image -lpthread -lm

# CONFIG
SOURCE=.
OUTPUT=bin

default: build

build: init
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/display.c -o $(OUTPUT)/display.o
	$(CC) $(DEBUG) $(OPTIONS) -c $(SOURCE)/utilities.c -o $(OUTPUT)/utilities.o
	$(CC) $(DEBUG) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/display.o $(OUTPUT)/utilities.o $(LIB)

debug: DEBUG = -DDEBUG
debug: build

init:
	mkdir -p bin

clean:
	rm -rf bin