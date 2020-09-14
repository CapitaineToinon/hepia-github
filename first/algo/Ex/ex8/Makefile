CC=gcc
OPTIONS=-g -O0 -W -Wall

# CONFIG
SOURCE=src
OUTPUT= bin

# DEFAULT
default: build

# Builds all the files
build: init
	$(CC) $(OPTIONS) -c $(SOURCE)/output.c -o $(OUTPUT)/output.o
	$(CC) $(OPTIONS) -c $(SOURCE)/input.c -o $(OUTPUT)/input.o
	$(CC) $(OPTIONS) -c $(SOURCE)/connect_four.c -o $(OUTPUT)/connect_four.o
	$(CC) $(OPTIONS) -c $(SOURCE)/player_vs_cpu.c -o $(OUTPUT)/player_vs_cpu.o
	$(CC) $(OPTIONS) -c $(SOURCE)/player_vs_player.c -o $(OUTPUT)/player_vs_player.o
	$(CC) $(OPTIONS) $(SOURCE)/main.c -o $(OUTPUT)/main $(OUTPUT)/output.o $(OUTPUT)/input.o $(OUTPUT)/player_vs_cpu.o $(OUTPUT)/connect_four.o $(OUTPUT)/player_vs_player.o

# Deletes the OUTPUT folder
clean: 
	rm -rf $(OUTPUT)

# Creates the OUTPUT folder
init: 
	mkdir -p $(OUTPUT)

# Default
.PHONY : default