#ifndef __WSH_H_SEEN__
#define __WSH_H_SEEN__

#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "colors.h"
#include "builtins.h"
#include "jobs.h"

#define WSH_RL_BUFFERSIZE 1024
#define WSH_SHELL_LOGO "ヅ"
#define WSH_TOK_SEP " \t\r\n\a"
#define WSH_TOK_BUFFERSIZE 64
#define WSH_SHELL_NAME "wsh"
#define WSH_EXIT_MESSAGE "N... not like I enjoyed talking with you, baka!\n"
#define WSH_JOB_EXIT "Foreground job exited with code %d.\n"
#define WSH_BACKGROUND_JOB_EXIT "\nBackground job with pid [%d] exited with code %d\n"
#define WSH_SUPPORTED_ACTIONS "&, |, >, >>"

typedef enum cmdtype
{
    CMD_SIMPLE,
    CMD_PIPE,
    CMD_FILEOUT,
    CMD_FILEIN,
    CMD_FILEAPPEND,
    CMD_BACKGROUND,
} cmdtype_t;

/**
 * Parses arguments and return the type of the command and if possible, finds where the 
 * @param int argc: the amount of arguments
 * @param char** argv: pointer to the arguments
 * @param int* split_index: the index at which the arguments should be split in the case where the command type needs it
 * @param cmdtype_t* type: the type of the command
 * @return 0 if the parsing was successful
 */
int wsh_get_command_type(int, char **, int *, cmdtype_t *);

/**
 * Pipes one command into another one
 * @param int argc: the amount of total arguments (argc of the first command + argc of the second command + 1 for the pipe character)
 * @param char** argv: pointer to all arguments (args of the first command followed by the pipe character followed by the args of the second command)
 * @param int split_index: the position of the pipe character in the argv. That index is returned by wsh_get_command_type()
 * @return the status code last command executed
 */
int wsh_process_pipe(int, char **, int);

/**
 * Redirects the output of a command to a file
 * @param int argc: the amount of total arguments (argc of the first command + 1 for the > or >> character + unkown amount of extra params)
 * @param char** argv: pointer to all arguments (args of the first command followed by the pipe character followed by one or more files to output the file)
 * @param int split_index: the position of the > or >> character in the argv. That index is returned by wsh_get_command_type()
 * @param bool append: whether the output should be appened to the end of the file or not
 * @return the status code of the command executed
 */
int wsh_process_fileout(int, char **, int, bool);

/**
 * Reads the user input until the user types enter
 * @return the user input as a string
 */
char *wsh_real_line();

/**
 * Splits a string by spaces
 * @param char* the string to be split
 * @param int* the size of the returned array
 * @return an array containing each segments
 */
char **wsh_split_line(char *, int *);

/**
 * Execute a command
 * @param int argc: the amount of arguments
 * @param char** argv: pointer to the arguments
 * @param bool whether the command should be ran in the background or not
 * @return the status code of the command executed
 */
int wsh_launch(int, char **, bool);

/**
 * Execute a command, being a builtin command or not. If the command is not a builtin, will call wsh_launch
 * @param int argc: the amount of arguments
 * @param char** argv: pointer to the arguments
 * @param bool whether the command should be ran in the background or not
 * @return the status code of the command executed. Will return -1 if background is true with a builtin command
 */
int wsh_execute(int, char **, bool);

/**
 * Main shell loop
 */
void wsh_loop();

#endif