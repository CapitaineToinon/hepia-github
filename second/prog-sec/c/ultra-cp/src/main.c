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

int main(int argc, char *argv[])
{
    char *argv_extra[argc - optind];
    int argc_extra = 0;
    bool a_flag = false;
    bool f_flag = false;
    int opt;

    // Read the program's arguments
    while ((opt = getopt(argc, argv, ":af")) != -1)
    {
        switch (opt)
        {
        case 'a':
            a_flag = true;
            break;
        case 'f':
            f_flag = true;
            break;
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
        fprintf(stderr, "Example usage: %s <directory_or_file>\n", argv[0]);
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

        if ((!file_exists(destination) || !is_directory(destination)) && is_directory(source))
        {
            fprintf(stderr, "If the destination is a file, you can have only one source and it must be a file too.\n");
            exit(EXIT_FAILURE);
        }

        if (!is_directory(source))
        {
            copy_file(source, destination, a_flag, f_flag);
        }
        else
        {
            copy_files(source, destination, a_flag, f_flag);
        }
    }
    else
    {
        /**
         * With 3 or more arguements, copy files.
         * If the destination doesn't exist OR is a file, then throws an error
         * If the destination is a directory, then sources can be a file or a directory
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

            if (!is_directory(source))
            {
                copy_file(source, destination, a_flag, f_flag);
            }
            else
            {
                copy_files(source, destination, a_flag, f_flag);
            }
        }
    }

    return 0;
}