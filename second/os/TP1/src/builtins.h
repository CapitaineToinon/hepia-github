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

/**
 * Find a builtin command by its name and assign to to *command if found
 * @param char* name of the command
 * @returns whether the builtin was found or not
 */
bool find_builtin(char *, Command *);

/**
 * Tests by name if a command is a builtin or not
 * @param char* name of the command
 * @returns whether the command is a builtin or not
 */
bool is_builtin(char *);

/**
 * Prints the shell's help command
 * @param int argc: the amount of arguments
 * @param char** argv: pointer to the arguments
 * @returns status code
 */
int wsh_help(int, char **);

/**
 * Builtin implementation of the command "exit"
 * @param int argc: the amount of arguments
 * @param char** argv: pointer to the arguments
 * @returns status code
 */
int wsh_exit(int, char **);

/**
 * Builtin implementation of the command "cd"
 * @param int argc: the amount of arguments
 * @param char** argv: pointer to the arguments
 * @returns status code
 */
int wsh_cd(int, char **);

/**
 * Builtin implementation of the command "type"
 * @param int argc: the amount of arguments
 * @param char** argv: pointer to the arguments
 * @returns status code
 */
int wsh_type(int, char **);

/**
 * Says hi
 * @param int argc: the amount of arguments
 * @param char** argv: pointer to the arguments
 * @returns status code
 */
int wsh_hi(int, char **);

#endif