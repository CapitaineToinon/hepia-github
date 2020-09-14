CC=gcc
COPTS= -g -ansi -lm
LINT=splint
LINTOPTS= -redef

# CONFIG
SOURCE=src
OUTPUT= bin
SOURCES := $(wildcard $(SOURCE)/*.c)

# DEFAULT
default: build

# Builds all the files
build: build_$(SOURCES)

# Lints all the files
lint: 
	$(LINT) $(LINTOPTS) $(SOURCE)/*.c

# Deletes the OUTPUT folder
clean: 
	rm -rf $(OUTPUT)

# Creates the OUTPUT folder
init: 
	mkdir -p $(OUTPUT)

# Builds all the files
build_$(SOURCES): $@ clean init
	$(CC) src/$(notdir $(basename $@)).c $(COPTS) -o bin/$(notdir $(basename $@))

# Default
.PHONY : default