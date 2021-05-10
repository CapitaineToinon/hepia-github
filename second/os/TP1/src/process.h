#ifndef __PROCESS_H_SEEN__
#define __PROCESS_H_SEEN__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "wsh.h"

typedef enum cmdtype
{
    CMD_SIMPLE,
    CMD_PIPE,
    CMD_FILEOUT,
    CMD_FILEIN,
    CMD_FILEAPPEND
} cmdtype_t;

cmdtype_t get_command_type(int, char **, int *);
int process_pipe(int, char **, int);
int process_fileout(int, char **, int, bool);

#endif