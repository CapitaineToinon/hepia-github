#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "file.h"

bool is_directory(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

void copy_file(const char *source, char *destination)
{
    printf("%s -> %s\n", source, destination);
}

void foreach_file(const char *dir_name, void (*ptr)(char *))
{
    struct dirent *entry;
    DIR *dir = opendir(dir_name);

    if (dir == NULL)
    {
        fprintf(stderr, "Count not open directory %s: %s\n", dir_name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, CURRENT_DIR) == 0 || strcmp(entry->d_name, PARENT_DIR) == 0)
            continue;

        char full_path[PATH_MAX];
        snprintf(full_path, PATH_MAX, "%s/%s", dir_name, entry->d_name);

        (*ptr)(full_path);

        if (entry->d_type & DT_DIR)
        {
            foreach_file(full_path, ptr);
        }
    }

    closedir(dir);
}

void print_file_info(char *file)
{
    struct stat ret;
    long int size = 0;
    char time[DATE_SIZE];
    char stats[] = DEFAULT_PERM;

    if (stat(file, &ret) != -1)
    {
        size = ret.st_size;
        strftime(time, DATE_SIZE, DATE_FORMAT, localtime(&ret.st_mtime));
        snprintf(stats, strlen(stats), "%s%s%s%s%s%s%s%s%s%s",
                 S_ISDIR(ret.st_mode) ? "d" : "-",
                 (ret.st_mode & S_IRUSR) ? "r" : "-",
                 (ret.st_mode & S_IWUSR) ? "w" : "-",
                 (ret.st_mode & S_IXUSR) ? "x" : "-",
                 (ret.st_mode & S_IRGRP) ? "r" : "-",
                 (ret.st_mode & S_IRGRP) ? "w" : "-",
                 (ret.st_mode & S_IRGRP) ? "x" : "-",
                 (ret.st_mode & S_IROTH) ? "r" : "-",
                 (ret.st_mode & S_IROTH) ? "w" : "-",
                 (ret.st_mode & S_IROTH) ? "x" : "-");
    }

    printf("%10s %10ld %10s %s\n", stats, size, time, file);
}