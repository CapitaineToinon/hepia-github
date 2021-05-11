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
#include "wsh.h"

#define WSH_RL_BUFFERSIZE 1024
#define WSH_SHELL_LOGO "ヅ"
#define WSH_TOK_SEP " \t\r\n\a"
#define WSH_TOK_BUFFERSIZE 64
#define WSH_SHELL_NAME "wsh"
#define WSH_EXIT_MESSAGE "N... not like I enjoyed talking with you, baka!\n"
#define WSH_JOB_EXIT "Foreground job exited with code %d.\n"

typedef enum cmdtype
{
    CMD_SIMPLE,
    CMD_PIPE,
    CMD_FILEOUT,
    CMD_FILEIN,
    CMD_FILEAPPEND
} cmdtype_t;

cmdtype_t wsh_get_command_type(int, char **, int *);
int wsh_process_pipe(int, char **, int);
int wsh_process_fileout(int, char **, int, bool);
int wsh_launch(int, char **);
char *wsh_real_line();
char **wsh_split_line(char *, int *);
void wsh_welcome();
int wsh_execute(int, char **);
void wsh_loop();

#endif