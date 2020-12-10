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

#define CURRENT_DIR "."
#define PARENT_DIR ".."
#define DEFAULT_PERM "----------"
#define DATE_FORMAT "%a %b %e %H:%M:%S %G"
#define DATE_SIZE 50

void printHelp(char *file_name)
{
    printf("Example usage: %s <directory_or_file>\n", file_name);
}

void foreach_file(const char *dir_name, void (*ptr)(char *))
{
    DIR *dir = opendir(dir_name);
    struct dirent *entry;

    if (dir == NULL)
    {
        fprintf(stderr, "Could not open directory '%s': %s\n.", dir_name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, CURRENT_DIR) == 0 || strcmp(entry->d_name, PARENT_DIR) == 0)
            continue;

        char fullPath[PATH_MAX];
        snprintf(fullPath, PATH_MAX, "%s/%s", dir_name, entry->d_name);

        (*ptr)(fullPath); // callback

        if (entry->d_type & DT_DIR)
        {
            foreach_file(fullPath, ptr);
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

void copy_file(char *source, char *destination)
{
    printf("%s -> %s\n", source, destination);
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printHelp(argv[0]);
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s", argv[1]);
        foreach_file(path, &print_file_info);
    }
    else
    {
        char destination[PATH_MAX];
        snprintf(destination, PATH_MAX, "%s", argv[argc - 1]);

        void call_copy(char *source)
        {
            copy_file(source, destination);
        }

        for (int i = 1; i < argc - 1; i++)
        {
            char source[PATH_MAX];
            snprintf(source, PATH_MAX, "%s", argv[i]);
            foreach_file(source, &call_copy);
        }

        foreach_file(destination, &print_file_info);
    }

    return 0;
}