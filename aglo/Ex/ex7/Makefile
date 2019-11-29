CC=gcc

# CONFIG
SOURCE=src
OUTPUT= bin

# DEFAULT
default: build

# Builds all the files
build: init
	$(CC) $(SOURCE)/main.c -o $(OUTPUT)/main

# Deletes the OUTPUT folder
clean: 
	rm -rf $(OUTPUT)

# Creates the OUTPUT folder
init: 
	mkdir -p $(OUTPUT)

# Default
.PHONY : default