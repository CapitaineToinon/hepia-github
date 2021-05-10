#include "process.h"

cmdtype_t get_command_type(int argc, char **args, int *split_index)
{
    *split_index = 0;

    for (int i = 0; i < argc; i++)
    {
        // we need to do tests using strncmp first
        if (strncmp(args[i], ">>", 2) == 0)
        {
            *split_index = i;
            return CMD_FILEAPPEND;
        }

        if (*args[i] == '|')
        {
            *split_index = i;
            return CMD_PIPE;
        }

        if (*args[i] == '>')
        {
            *split_index = i;
            return CMD_FILEOUT;
        }
    }

    // if we reach this, it's a simple command
    return CMD_SIMPLE;
}

int process_pipe(int argc, char **args, int split_index)
{
    char **first_args = malloc(split_index * sizeof(char *));
    memcpy(first_args, args, split_index * sizeof(char *));
    // wsh_execute(split_index, first_args);
    wsh_execute(split_index + 1, args + split_index);
    return 0;
}

int process_fileout(int argc, char **args, int split_index, bool append)
{
    if (argc - (split_index + 1) != 1)
    {
        fprintf(stderr, "Output of fileout invalid.\n");
        return EXIT_FAILURE;
    }

    int fd;
    char *output = args[argc - 1];

    int first_argc = split_index + 1;
    char **first_args = malloc(split_index * sizeof(char *));
    memcpy(first_args, args, split_index * sizeof(char *));

    // save stdout and error to be restored after we're done redirecting
    // output to a file
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);

    // different perms if we append or not
    int extra_perms = append ? O_APPEND : O_TRUNC;

    if ((fd = open(output, O_RDWR | O_CREAT | extra_perms, S_IRUSR | S_IWUSR)) == -1)
    {
        fprintf(stderr, "Failed to open in process_fileout: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    wsh_execute(first_argc, first_args);
    fflush(stdout);

    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);

    close(fd);
    free(first_args);
    return EXIT_SUCCESS;
}