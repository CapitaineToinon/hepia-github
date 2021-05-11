#include "wsh.h"

// declared in builtins.c
extern const Command BUILTINS[];
extern const int BUILTINS_COUNT;

volatile sig_atomic_t sigint_signal;

static void sigint_handler()
{
    /* Take appropriate actions for signal delivery */
    printf("\n");
    sigint_signal = 1;
}

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

    // check if any of the command is a builtin and exit if it is, we don't handle those
    if (is_builtin(first_args[0]) || is_builtin(second_args[0]))
    {
        free(first_args);
        fprintf(stderr, "Using builtins with pipes is not supported.\n");
        return EXIT_FAILURE;
    }

    // now handle the pipe process
    pid_t pid;
    int WRITE_END = 0;
    int READ_END = 1;
    int fd[2];
    pipe(fd);

    pid = fork();

    if (pid < 0)
    {
        // Error forking
        free(first_args);
        perror(WSH_SHELL_NAME);
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        // We're now in the child process.
        // set pgid to avoid zombie process
        if (setpgid(pid, 0) == -1)
        {
            free(first_args);
            perror(WSH_SHELL_NAME);
            return EXIT_FAILURE;
        }

        // redirect stdout to a file descriptor we control for later
        dup2(fd[READ_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);

        // execute the first command
        execvp(first_args[0], first_args);

        // if we're still here, there was an error
        free(first_args);
        perror(WSH_SHELL_NAME);
        exit(EXIT_FAILURE);
    }
    else
    {
        pid = fork();

        if (pid < 0)
        {
            // Error forking
            free(first_args);
            perror(WSH_SHELL_NAME);
            return EXIT_FAILURE;
        }

        if (pid == 0)
        {
            // We're now in the child process.
            // set pgid to avoid zombie process
            if (setpgid(pid, 0) == -1)
            {
                perror(WSH_SHELL_NAME);
                return EXIT_FAILURE;
            }

            dup2(fd[WRITE_END], STDIN_FILENO);
            close(fd[WRITE_END]);
            close(fd[READ_END]);

            // execute the second command
            execvp(second_args[0], second_args);

            // if we're still here, there was an error
            free(first_args);
            perror(WSH_SHELL_NAME);
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
                free(first_args);
                perror(WSH_SHELL_NAME);
                return EXIT_SUCCESS;
            }

            if (WIFEXITED(status))
            {
                free(first_args);
                return WEXITSTATUS(status);
            }

            if (WIFSIGNALED(status))
            {
                free(first_args);
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
        // set pgid to avoid zombie process
        if (setpgid(pid, 0) == -1)
        {
            perror(WSH_SHELL_NAME);
            return EXIT_FAILURE;
        }

        // Nothing below execvp should execute
        // if execvp call is succesful
        execvp(args[0], args);

        // if we're still here, there was an error
        perror(WSH_SHELL_NAME);
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
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        fprintf(stderr, "Failed to sigaction in main: %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    char *line;
    char **args;
    char pwd[PATH_MAX];
    int argc;
    int code = 0;

    while (true)
    {
        sigint_signal = 0;
        getcwd(pwd, PATH_MAX);
        char *color = code == 0 ? BOLDGREEN : BOLDRED;
        printf("%s>" RESET " " BOLDMAGENTA "%s" RESET ":" BOLDBLACK " %s " RESET, color, pwd, WSH_SHELL_LOGO);

        line = wsh_real_line();
        args = wsh_split_line(line, &argc);

        int split_index;
        cmdtype_t type = wsh_get_command_type(argc, args, &split_index);

        sa.sa_flags = SA_RESTART;

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

        printf(WSH_JOB_EXIT, code);
        free(line);
        free(args);
    }
}