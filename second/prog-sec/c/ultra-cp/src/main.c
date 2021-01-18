#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "file.h"

void print_help(FILE *, char *);
void print_help(FILE *dest, char *filename)
{
    fprintf(dest, "Usage: %s [OPTIONS]... [SOURCE] DEST\n", filename);
    fprintf(dest, "or:    %s [OPTIONS]... SOURCE DEST\n", filename);
    fprintf(dest, "Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.\n");
    fprintf(dest, "-a        Force the copy of permissions.\n");
    fprintf(dest, "-f        Creates new symbolic links instead of copying the content of the link.\n");
    fprintf(dest, "-h        Prints this help.\n");
}

int main(int argc, char *argv[])
{
    char *argv_extra[argc];
    int argc_extra = 0;
    bool a_flag = false;
    bool f_flag = false;
    int opt;

    // Read the program's arguments
    while ((opt = getopt(argc, argv, ":afh")) != -1)
    {
        switch (opt)
        {
        case 'a':
            a_flag = true;
            break;
        case 'f':
            f_flag = true;
            break;
        case 'h':
            print_help(stdout, argv[0]);
            return 0;
        }
    }

    // Populate the array of extra arguments
    for (; optind < argc; optind++)
    {
        argv_extra[argc_extra++] = argv[optind];
    }

    // start of the main program
    if (argc_extra == 0)
    {
        // No arguements provided
        print_help(stderr, argv[0]);
        exit(EXIT_FAILURE);
    }
    else if (argc_extra == 1)
    {
        // Only one arguement, simple list the files
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s", argv_extra[0]);
        print_files_info(path);
    }
    else if (argc_extra == 2)
    {
        /**
         * With 2 arguements, copy files.
         * If the destination doesn't exist OR is a file, then the source needs to also be a file
         * If the destination is a directory, then source can be a file or a directory
         */
        char source[PATH_MAX];
        char destination[PATH_MAX];
        snprintf(source, PATH_MAX, "%s", argv_extra[0]);
        snprintf(destination, PATH_MAX, "%s", argv_extra[1]);

        if (is_directory(source))
        {
            if (!file_exists(destination) || !is_directory(destination))
            {
                fprintf(stderr, "If the destination is a file, you can have only one source and it must be a file too.\n");
                exit(EXIT_FAILURE);
            }

            copy_files(source, destination, a_flag, f_flag);
        }
        else
        {
            copy_file(source, destination, a_flag, f_flag);
        }
    }
    else
    {
        /**
         * With 3 or more arguements, copy files.
         * If the destination doesn't exist OR is a file, then throws an error
         * Sources can be a file or a directory
         */
        char source[PATH_MAX];
        char destination[PATH_MAX];
        snprintf(destination, PATH_MAX, "%s", argv_extra[argc_extra - 1]);

        if (!file_exists(destination) || !is_directory(destination))
        {
            fprintf(stderr, "When copying more than one source, the destination needs to be a folder\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < argc_extra - 1; i++)
        {
            snprintf(source, PATH_MAX, "%s", argv_extra[i]);

            if (is_directory(source))
            {
                copy_files(source, destination, a_flag, f_flag);
            }
            else
            {
                copy_file(source, destination, a_flag, f_flag);
            }
        }
    }

    return 0;
}