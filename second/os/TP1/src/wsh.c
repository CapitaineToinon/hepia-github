#include "wsh.h"

// declared in builtins.c
extern const Command BUILTINS[];
extern const int BUILTINS_COUNT;

cmdtype_t wsh_get_command_type(int argc, char **args, int *split_index)
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

int wsh_process_pipe(int argc, char **args, int split_index)
{
    (void)argc;
    int status = EXIT_FAILURE;

    // create a new args for the first command
    char **first_args;
    char **second_args = args + split_index + 1;

    if ((first_args = malloc(split_index * sizeof(char *))) == NULL)
    {
        fprintf(stderr, "Failed to malloc in wsh_process_pipe: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memcpy(first_args, args, split_index * sizeof(char *));

    // now handle the pipe process
    pid_t pid;
    int WRITE_END = 0;
    int READ_END = 1;
    int fd[2];
    pipe(fd);

    if ((pid = fork()) == 0)
    {
        // redirect stdout to a file descriptor we control for later
        dup2(fd[READ_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);

        // execute the first command
        if (execvp(first_args[0], first_args) == -1)
        {
            perror(WSH_SHELL_NAME);
        }

        // we should never reach this unless something failed
        exit(EXIT_FAILURE);
    }
    else
    {
        if ((pid = fork()) == 0)
        {
            dup2(fd[WRITE_END], STDIN_FILENO);
            close(fd[WRITE_END]);
            close(fd[READ_END]);

            // execute the second command
            if (execvp(second_args[0], second_args) == -1)
            {
                perror(WSH_SHELL_NAME);
            }

            // we should never reach this unless something failed
            exit(EXIT_FAILURE);
        }
        else
        {
            // both commands were executed, now wait
            // for the status code
            close(fd[READ_END]);
            close(fd[WRITE_END]);
            pid_t wait = waitpid(pid, &status, WUNTRACED);

            if (wait < 0)
            {
                perror(WSH_SHELL_NAME);
                return EXIT_FAILURE;
            }

            if (WIFEXITED(status))
            {
                return WEXITSTATUS(status);
            }

            if (WIFSIGNALED(status))
            {
                return WIFSIGNALED(status);
            }
        }
    }

    free(first_args);
    return status;
}

int wsh_process_fileout(int argc, char **args, int split_index, bool append)
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

    // different perms if we append or not
    int extra_perms = append ? O_APPEND : O_TRUNC;

    if ((fd = open(output, O_RDWR | O_CREAT | extra_perms, S_IRUSR | S_IWUSR)) == -1)
    {
        fprintf(stderr, "Failed to open in wsh_process_fileout: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        fprintf(stderr, "Error with dup2() in wsh_process_fileout: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    int status = wsh_execute(first_argc, first_args);
    fflush(stdout);

    if (dup2(saved_stdout, STDOUT_FILENO) == -1)
    {
        fprintf(stderr, "Error with dup2() in wsh_process_fileout: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    close(fd);
    free(first_args);
    return status;
}

int wsh_launch(int argc, char **args)
{
    (void)argc;
    int status;
    pid_t pid = fork();

    if (pid < 0)
    {
        // Error forking
        perror(WSH_SHELL_NAME);
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        // We're now in the child process.
        // Nothing below execvp should execute
        // if execvp call is succesful
        if (execvp(args[0], args) == -1)
        {
            perror(WSH_SHELL_NAME);
        }

        // we should never reach this unless something failed
        exit(EXIT_FAILURE);
    }
    else
    {
        // We're in the parent process, now wait for the
        // child process to return
        pid_t wait = waitpid(pid, &status, WUNTRACED);

        if (wait < 0)
        {
            perror(WSH_SHELL_NAME);
            return EXIT_FAILURE;
        }

        if (WIFEXITED(status))
        {
            return WEXITSTATUS(status);
        }

        if (WIFSIGNALED(status))
        {
            return WIFSIGNALED(status);
        }

        return status;
    }

    return EXIT_FAILURE;
}

char *wsh_real_line()
{
    int position = 0;
    char *buffer;
    char c;

    if ((buffer = (char *)malloc(sizeof(char) * WSH_RL_BUFFERSIZE)) == NULL)
    {
        fprintf(stderr, "Failed to malloc buffer in wsh_real_line.\n");
        exit(EXIT_FAILURE);
    }

    while (true && position < WSH_RL_BUFFERSIZE)
    {
        c = getchar();

        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            break;
        }
        else
        {
            buffer[position] = c;
        }

        position++;
    }

    return buffer;
}

char **wsh_split_line(char *line, int *count)
{
    *count = 0;
    char *arg;
    char **args = NULL;

    arg = strtok(line, WSH_TOK_SEP);
    while (arg != NULL)
    {
        if ((args = realloc(args, sizeof(char *) * (*count + 1))) == NULL)
        {
            fprintf(stderr, "Failed to realloc buffer in wsh_split_line.\n");
            exit(EXIT_FAILURE);
        }

        args[*count] = arg;
        arg = strtok(NULL, WSH_TOK_SEP);
        (*count)++;
    }

    return args;
}

void wsh_welcome()
{
    printf(MAGENTA "Welcome to the weeb shell (wsh).\n" RESET);
}

int wsh_execute(int argc, char **args)
{
    if (args == NULL || args[0] == NULL)
    {
        return EXIT_SUCCESS;
    }

    // check for builtins first
    for (int i = 0; i < BUILTINS_COUNT; i++)
    {
        if (strcmp(BUILTINS[i].name, args[0]) == 0)
        {
            return BUILTINS[i].function(argc, args);
        }
    }

    return wsh_launch(argc, args);
}

void wsh_loop()
{
    char *line;
    char **args;
    char pwd[PATH_MAX];
    int argc;
    int code = 0;

    while (true)
    {
        getcwd(pwd, PATH_MAX);
        char *color = code == 0 ? BOLDGREEN : BOLDRED;
        printf("%s>" RESET " " BOLDMAGENTA "%s" RESET ":" BOLDBLACK " %s " RESET, color, pwd, WSH_SHELL_LOGO);

        line = wsh_real_line();
        args = wsh_split_line(line, &argc);

        int split_index;
        cmdtype_t type = wsh_get_command_type(argc, args, &split_index);

        switch (type)
        {
        case CMD_PIPE:
            code = wsh_process_pipe(argc, args, split_index);
            break;

        case CMD_FILEOUT:
            code = wsh_process_fileout(argc, args, split_index, false);
            break;

        case CMD_FILEAPPEND:
            code = wsh_process_fileout(argc, args, split_index, true);
            break;

        default:
        case CMD_SIMPLE:
            code = wsh_execute(argc, args);
            break;
        }

        free(line);
        free(args);
    }
}