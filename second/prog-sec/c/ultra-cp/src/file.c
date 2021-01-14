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
#include <libgen.h>
#include "file.h"

/**
 * Returns true if the file or directory exists
 */
bool file_exists(char *source)
{
    struct stat path_stat;
    return stat(source, &path_stat) == 0;
}

/**
 * Returns true if the string is the path to an existing directory
 */
bool is_directory(char *source)
{
    struct stat path_stat;
    return stat(source, &path_stat) == 0 && S_ISDIR(path_stat.st_mode);
}

/**
 * Returns true if the string ends with a forward slash
 */
bool has_trailing_slash(const char *source)
{
    int strLength = strlen(source);
    return (strLength > 0 && source[strLength - 1] == 47);
}

/**
 * Copies a file to a destination. Only copies the file if the source is newer than
 * the destination or if the source has a different amount of bytes than the destination.
 * 
 * source: the file to be copied
 * destination: the file destination or a directory the source file will be copied to
 * force_perms_copy: force to copy the permissions of the source to the destination
 */
void copy_file(const char *source, char *destination, bool force_perms_copy, bool copy_links)
{
    int fd_src;
    int fd_dest;
    char buffer[4096];
    ssize_t nread;
    struct stat source_stat;
    struct stat destination_stat;
    bool copied = false;

    // need a local copy to not mutate the parameter
    char _destination[PATH_MAX];
    snprintf(_destination, PATH_MAX, "%s", destination);

    // open the source
    if ((fd_src = open(source, O_RDONLY)) < 0)
    {
        fprintf(stderr, "Count not open file %s: %s\n", source, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (stat(source, &source_stat) != 0)
    {
        close(fd_src);
        fprintf(stderr, "Count not open file %s: %s\n", source, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // test if the destination is a file or a folder, changes
    // the destination to 'destination/source_basename' if needed
    if (is_directory(_destination))
    {
        // I'm not sure why I need a tmp variable here
        // directly assigning to _destination doesn't work
        char tmp[PATH_MAX];
        char *destin_next_path_format = has_trailing_slash(_destination) ? "%s%s" : "%s/%s";
        snprintf(tmp, PATH_MAX, destin_next_path_format, _destination, basename((char *)source));
        snprintf(_destination, PATH_MAX, "%s", tmp);
    }

    // open or create the destination
    if ((fd_dest = open(_destination, O_WRONLY | O_EXCL, 0600)) < 0)
    {
        // file doesn't exist so we create it
        if (errno == ENOENT && (fd_dest = open(_destination, O_WRONLY | O_CREAT | O_EXCL, 0600)) < 0)
        {
            close(fd_src);
            fprintf(stderr, "Count not open file %s: %s\n", _destination, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    if (stat(_destination, &destination_stat) != 0)
    {
        close(fd_src);
        close(fd_dest);
        fprintf(stderr, "Count not open file %s: %s\n", _destination, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Only copy file if source was last modified before destination
    // or if bytes count is different.
    // Silently do nothing otherwise.
    if (source_stat.st_mtime > destination_stat.st_mtime || source_stat.st_size != destination_stat.st_size)
    {
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
                    fprintf(stderr, "Could not copy file %s to %s: %s\n", source, _destination, strerror(errno));
                    exit(EXIT_FAILURE);
                }
            } while (nread > 0);
        }

        copied = true;
    }

    // Copy the permissions over if needed
    if (copied || force_perms_copy)
    {
        chmod(_destination, source_stat.st_mode);
    }

    // pretty print the result
    printf("%s -> %s\n", source, _destination);

    close(fd_src);
    close(fd_dest);
}

/**
 * Copies files from a source directory to a destination directory. Only copies the file if the source is newer than
 * the destination or if the source has a different amount of bytes than the destination.
 * 
 * Calls copy_file internally.
 * 
 * source: the source directory containing the files that will be copied over
 * destination: the destination directory the source files will be copied to
 * force_perms_copy: force to copy the permissions of the source to the destination
 */
void copy_files(const char *source, char *destination, bool force_perms_copy, bool copy_links)
{
    struct dirent *entry;
    DIR *dir = opendir(source);
    char *source_next_path_format = has_trailing_slash(source) ? "%s%s" : "%s/%s";
    char *destin_next_path_format = has_trailing_slash(destination) ? "%s%s" : "%s/%s";

    if (dir == NULL)
    {
        fprintf(stderr, "Count not open directory %s: %s\n", source, strerror(errno));
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, CURRENT_DIR) == 0 || strcmp(entry->d_name, PARENT_DIR) == 0)
            continue;

        char source_full_path[PATH_MAX];
        char destin_full_path[PATH_MAX];
        snprintf(source_full_path, PATH_MAX, source_next_path_format, source, entry->d_name);
        snprintf(destin_full_path, PATH_MAX, destin_next_path_format, destination, entry->d_name);

        if (entry->d_type & DT_DIR)
        {
            struct stat source_directory;

            if (stat(source_full_path, &source_directory) != 0)
            {
                closedir(dir);
                fprintf(stderr, "Count not open directory %s: %s\n", source, strerror(errno));
                exit(EXIT_FAILURE);
            }

            mkdir(destin_full_path, source_directory.st_mode);
            copy_files(source_full_path, destin_full_path, force_perms_copy, copy_links);
        }
        else
        {
            copy_file(source_full_path, destin_full_path, force_perms_copy, copy_links);
        }
    }

    closedir(dir);
}

/**
 * Calls print_file_info for a single file or for each files in a directory
 */
void print_files_info(char *dir_name)
{
    // Exit early if dir_name is actually a file
    if (!is_directory(dir_name))
    {
        print_file_info(dir_name);
        return;
    }

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

        print_file_info(full_path);

        if (entry->d_type & DT_DIR)
        {
            print_files_info(full_path);
        }
    }

    closedir(dir);
}

/**
 * Prints various info about a file.
 * 
 * Example:
 *  drwxrwxrw        128 Thu Jan  7 14:50:21 2021 ./hello
 */
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