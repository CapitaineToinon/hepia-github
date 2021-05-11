#ifndef __BUILTINS_H_SEEN__
#define __BUILTINS_H_SEEN__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include "colors.h"
#include "wsh.h"

typedef struct command_t
{
    char name[PATH_MAX];
    int (*function)(int, char **);
} Command;

bool is_builtin(char *);
int wsh_execute(int, char **);
int wsh_help(int, char **);
int wsh_exit(int, char **);
int wsh_cd(int, char **);
int wsh_type(int, char **);

#endif