CC=gcc
COPTS= -g -ansi -lm
LINT=splint
LINTOPTS= -redef

# CONFIG
SOURCE=src
OUTPUT= bin

default: clean init queen queens

queen: $(SOURCE)/queen/queen.c $(SOURCE)/queen/func.c $(SOURCE)/queen/func.h
	$(CC) -c $(SOURCE)/queen/queen.c $(COPTS) -o $(OUTPUT)/queen.o
	$(CC) -c $(SOURCE)/queen/func.c $(COPTS) -o $(OUTPUT)/func.o
	$(CC) $(COPTS) -o $(OUTPUT)/queen $(OUTPUT)/queen.o $(OUTPUT)/func.o

queens: $(SOURCE)/queens/queens.c
	$(CC) $(SOURCE)/queens/queens.c $(COPTS) -o $(OUTPUT)/queens 


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