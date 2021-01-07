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
    if (argc <= 1)
    {
        fprintf(stderr, "Example usage: %s <directory_or_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s", argv[1]);
        print_files_info(path);
    }
    else if (argc == 3)
    {
        char source[PATH_MAX];
        char destination[PATH_MAX];
        snprintf(source, PATH_MAX, "%s", argv[1]);
        snprintf(destination, PATH_MAX, "%s", argv[2]);

        if ((!file_exists(destination) || !is_directory(destination)) && is_directory(source))
        {
            fprintf(stderr, "If the destination is a file, you can have only one source and it must be a file too.\n");
            exit(EXIT_FAILURE);
        }

        if (is_directory(source))
        {
            // Nested function to call copy_file
            // since the foreach_file callback
            // only takes one param
            void call_copy(char *src)
            {
                copy_file(src, destination);
            }

            foreach_file(source, call_copy);
        }
        else
        {
            copy_file(source, destination);
        }
    }
    else
    {
        // todo
    }

    return 0;
}