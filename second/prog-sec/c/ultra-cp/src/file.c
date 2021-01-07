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
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "file.h"

bool file_exists(char *source)
{
    struct stat path_stat;
    return stat(source, &path_stat) == 0;
}

bool is_directory(char *source)
{
    struct stat path_stat;
    return stat(source, &path_stat) == 0 && S_ISDIR(path_stat.st_mode);
}

bool has_trailing_slash(const char *source)
{
    int strLength = strlen(source);
    return (strLength > 0 && source[strLength - 1] == 47);
}

void copy_file(const char *source, char *destination)
{
    int fd_src;
    int fd_dest;
    char buffer[4096];
    ssize_t nread;
    struct stat source_stat;
    struct stat destination_stat;
    bool destination_created = false;

    // open the source
    if ((fd_src = open(source, O_RDONLY)) < 0)
    {
        fprintf(stderr, "Count not open file %s: %s\n", source, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // open or create the destination
    if ((fd_dest = open(destination, O_WRONLY | O_EXCL, 0600)) < 0)
    {
        // file doesn't exist so we create it
        if (errno == ENOENT && (fd_dest = open(destination, O_WRONLY | O_CREAT | O_EXCL, 0600)) < 0)
        {
            close(fd_src);
            fprintf(stderr, "Count not open file %s: %s\n", destination, strerror(errno));
            exit(EXIT_FAILURE);
        }
        else
        {
            // file was created
            printf("created\n");
            destination_created = true;
        }
    }

    // If the destination already exists, we need to check some flags before copying
    if (!destination_created)
    {
        if (stat(source, &source_stat) != 0)
        {
            close(fd_src);
            close(fd_dest);
            fprintf(stderr, "Count not open file %s: %s\n", source, strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (stat(destination, &destination_stat) != 0)
        {
            close(fd_src);
            close(fd_dest);
            fprintf(stderr, "Count not open file %s: %s\n", destination, strerror(errno));
            exit(EXIT_FAILURE);
        }

        // @TODO ???? why this
        if (source_stat.st_mtime < destination_stat.st_mtime || source_stat.st_size == destination_stat.st_size)
        {
            return;
        }
    }

    // start the actual copy
    while (nread = read(fd_src, buffer, sizeof buffer), nread > 0)
    {
        char *new_ptr = buffer;
        ssize_t n_written;

        do
        {
            if ((n_written = write(fd_dest, new_ptr, nread)) >= 0)
            {
                nread -= n_written;
                new_ptr += n_written;
            }
            else
            {
                fprintf(stderr, "Could not copy file %s to %s: %s\n", source, destination, strerror(errno));
                exit(EXIT_FAILURE);
            }
        } while (nread > 0);
    }

    printf("%s -> %s\n", source, destination);
    close(fd_src);
    close(fd_dest);
}

void foreach_file(const char *dir_name, void (*ptr)(char *))
{
    struct dirent *entry;
    DIR *dir = opendir(dir_name);
    bool hasTrailing = has_trailing_slash(dir_name);
    char *next_path_format = hasTrailing ? "%s%s" : "%s/%s";

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
        snprintf(full_path, PATH_MAX, next_path_format, dir_name, entry->d_name);

        (*ptr)(full_path);

        if (entry->d_type & DT_DIR)
        {
            foreach_file(full_path, ptr);
        }
    }

    closedir(dir);
}

void print_files_info(char *file_or_dir)
{
    struct stat path_stat;

    if (stat(file_or_dir, &path_stat) != 0)
    {
        fprintf(stderr, "Count not open file or directory %s: %s\n", file_or_dir, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (S_ISDIR(path_stat.st_mode))
    {
        foreach_file(file_or_dir, print_file_info);
    }
    else
    {
        print_file_info(file_or_dir);
    }
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

        printf("%10s %10ld %10s %s\n", stats, size, time, file);
    }
    else
    {
        fprintf(stderr, "Count not open file %s: %s\n", file, strerror(errno));
        exit(EXIT_FAILURE);
    }
}