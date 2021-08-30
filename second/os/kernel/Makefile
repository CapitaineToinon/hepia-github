CC=gcc
OPTIONS=-O0 -W -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined -fsanitize=leak -g

obj-m += hello-4.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

abcd-fopen:
	$(CC) $(OPTIONS) abcd-fopen.c -o abcd-fopen

.PHONY: all test abcd-fopen