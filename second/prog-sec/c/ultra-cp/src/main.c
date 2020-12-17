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

        if (is_directory(path))
            foreach_file(path, &print_file_info);
        else
            print_file_info(path);
    }
    else
    {
        char destination[PATH_MAX];
        snprintf(destination, PATH_MAX, "%s", argv[argc - 1]);

        void call_copy(char *file)
        {
            copy_file(file, destination);
        }

        for (int i = 1; i < argc - 1; i++)
        {
            char source[PATH_MAX];
            snprintf(source, PATH_MAX, "%s", argv[i]);

            if (is_directory(source))
                foreach_file(source, &call_copy);
            else
                call_copy(source);
        }
    }

    return 0;
}