#include "wsh.h"

int wsh_get_command_type(int argc, char **args, int *split_index, cmdtype_t *type)
{
    *type = CMD_SIMPLE;
    int matches = 0;
    *split_index = 0;

    // check for background first
    if (argc > 0 && *args[argc - 1] == '&')
    {
        *type = CMD_BACKGROUND;
        matches++;
    }

    for (int i = 1; i < argc - 1; i++)
    {
        // we need to do tests using strncmp first
        if (strncmp(args[i], ">>", 2) == 0)
        {
            *split_index = i;
            *type = CMD_FILEAPPEND;
            matches++;
        }

        if (*args[i] == '|')
        {
            *split_index = i;
            *type = CMD_PIPE;
            matches++;
        }

        if (*args[i] == '>')
        {
            *split_index = i;
            *type = CMD_FILEOUT;
            matches++;
        }
    }

    // make sure the command contains only up to 1 action at a time
    // this is an intended limitation to keep the logic simple
    if (matches > 1)
    {
        fprintf(stderr, "You can only use one special action at a time per line (%s).\n", WSH_SUPPORTED_ACTIONS);
        return EXIT_FAILURE;
    }

#ifdef DEBUG
    const char *cmdtype_string[] = {
        "CMD_SIMPLE",
        "CMD_PIPE",
        "CMD_FILEOUT",
        "CMD_FILEIN",
        "CMD_FILEAPPEND",
        "CMD_BACKGROUND",
    };
    printf("[DEBUG]: Command of type %s\n", cmdtype_string[(int)*type]);
#endif

    return EXIT_SUCCESS;
}

int wsh_process_pipe(int argc, char **args, int split_index)
{
    (void)argc;
    int status = EXIT_FAILURE;

    char **first_args;                           // create with malloc a new args for the first command
    char **second_args = args + split_index + 1; // can just offset for the 2nd args

    if ((first_args = malloc(split_index * sizeof(char *))) == NULL)
    {
        fprintf(stderr, "Failed to malloc in wsh_process_pipe: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // build the first arguments, no need for the 2nd args
    memcpy(first_args, args, split_index * sizeof(char *));

    // check if any of the command is a builtin and exit if it is, we don't handle those
    if (is_builtin(first_args[0]) || is_builtin(second_args[0]))
    {
        fprintf(stderr, "Using builtins with pipes is not supported.\n");
        return EXIT_FAILURE;
    }

    // now handle the pipe process
    pid_t pid;
    int WRITE_END = 0;
    int READ_END = 1;
    int fd[2];
    pipe(fd);

    // fork for the first command
    pid = fork();

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
            exit(EXIT_FAILURE);
        }

        // redirect stdout to a file descriptor we control for later
        dup2(fd[READ_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);

        // execute the first command
        execvp(first_args[0], first_args);

        // if we're still here, there was an error
        perror(WSH_SHELL_NAME);
        exit(EXIT_FAILURE); // when in child, exit and not return to end the process
    }
    else
    {
        // first, wait for the first child process to be done
        if (waitpid(pid, NULL, WUNTRACED) < 0)
        {
            perror(WSH_SHELL_NAME);
            return EXIT_FAILURE;
        }

        // fork again for the 2nd command
        pid = fork();

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
                exit(EXIT_FAILURE);
            }

            dup2(fd[WRITE_END], STDIN_FILENO);
            close(fd[WRITE_END]);
            close(fd[READ_END]);

            // execute the second command
            execvp(second_args[0], second_args);

            // if we're still here, there was an error
            perror(WSH_SHELL_NAME);
            exit(EXIT_FAILURE); // when in child, exit and not return to end the process
        }
        else
        {
            // both commands were executed, now wait
            // for the status code
            close(fd[READ_END]);
            close(fd[WRITE_END]);

            if (waitpid(pid, &status, WUNTRACED) < 0)
            {
                perror(WSH_SHELL_NAME);
                status = EXIT_FAILURE;
            }

            if (WIFEXITED(status))
            {
                status = WEXITSTATUS(status);
            }

            if (WIFSIGNALED(status))
            {
                status = WIFSIGNALED(status);
            }
        }
    }

    free(first_args);
    return status;
}

int wsh_process_fileout(int argc, char **args, int split_index, bool append)
{
    int fd = -1;
    int status = EXIT_FAILURE;
    int first_argc = split_index + 1;
    char **first_args = NULL;
    char *output = NULL;

    if (argc - (split_index + 1) != 1)
    {
        fprintf(stderr, "Output of fileout invalid.\n");
        return EXIT_FAILURE;
    }

    output = args[argc - 1];
    first_args = malloc(split_index * sizeof(char *));
    memcpy(first_args, args, split_index * sizeof(char *));

    // save stdout and error to be restored after we're done redirecting
    // output to a file
    int saved_stdout = dup(STDOUT_FILENO);

    // different perms if we append or not
    int extra_perms = append ? O_APPEND : O_TRUNC;

    if ((fd = open(output, O_RDWR | O_CREAT | extra_perms, S_IRUSR | S_IWUSR)) == -1)
    {
        fprintf(stderr, "Failed to open in wsh_process_fileout: %s.\n", strerror(errno));
        free(first_args);
        return EXIT_FAILURE;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        fprintf(stderr, "Error with dup2() in wsh_process_fileout: %s.\n", strerror(errno));
        close(fd);
        free(first_args);
        return EXIT_FAILURE;
    }

    // execute the job that will be redirected to the output file
    status = wsh_execute(first_argc, first_args, false);

    if (fflush(stdout) != 0)
    {
        fprintf(stderr, "Error with fflush() in wsh_process_fileout: %s.\n", strerror(errno));
        status = EXIT_FAILURE;
    }

    if (dup2(saved_stdout, STDOUT_FILENO) == -1)
    {
        fprintf(stderr, "Error with dup2() in wsh_process_fileout: %s.\n", strerror(errno));
        status = EXIT_FAILURE;
    }

    close(fd);
    free(first_args);
    return status;
}

int wsh_launch(int argc, char **args, bool background)
{
    (void)argc;

    int status = EXIT_FAILURE;
    pid_t pid;

    // used for background jobs
    int fd = -1;
    int saved_stdout = 0;

    if (background && has_background_job())
    {
        fprintf(stderr, "A background job is already running. There can only be one background job at a time.\n");
        return EXIT_FAILURE;
    }

    // fork to execute the job
    pid = fork();

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
            exit(EXIT_FAILURE);
        }

        // redirect output if we background
        if (background)
        {
            saved_stdout = dup(STDOUT_FILENO);

            if ((fd = open("/dev/null", O_RDWR, S_IRUSR | S_IWUSR)) == -1)
            {
                fprintf(stderr, "Failed to open in wsh_launch: %s.\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                close(fd);
                fprintf(stderr, "Error with dup2() in wsh_launch: %s.\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
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
        // exit without waiting if we're in background mode
        if (background)
        {
            status = EXIT_SUCCESS;

            // save background job in case we have a sigout
            // no need the index
            if (!save_background_job(pid))
            {
                fprintf(stderr, "Failed to save as a background job but job already started. Keeps executing.\n");
                status = EXIT_FAILURE;
            }

            if (fflush(stdout) != 0)
            {
                fprintf(stderr, "Error with fflush() in wsh_process_fileout: %s.\n", strerror(errno));
                status = EXIT_FAILURE;
            }

            if (saved_stdout && dup2(saved_stdout, STDOUT_FILENO) == -1)
            {
                fprintf(stderr, "Error with dup2() in wsh_launch: %s.\n", strerror(errno));
                status = EXIT_FAILURE;
            }

            if (fd != -1)
            {
                close(fd);
            }

            printf("Started background job with pid [%d]\n", pid);
            return status;
        }

        // only save foreground jobs for potential sigint
        int index = save_foreground_job(pid);

        if (index == -1)
        {
            fprintf(stderr, "There are too many jobs already running.\n");
            return EXIT_FAILURE;
        }

        // We're in the parent process, now wait for the
        // child process to return
        if (waitpid(pid, &status, WUNTRACED) < 0)
        {
            perror(WSH_SHELL_NAME);
            status = EXIT_FAILURE;
        }

        if (WIFEXITED(status))
        {
            status = WEXITSTATUS(status);
        }

        if (WIFSIGNALED(status))
        {
            status = WIFSIGNALED(status);
        }

        free_job(index);
        return status;
    }

    return status;
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

int wsh_execute(int argc, char **args, bool background)
{
    if (args == NULL || args[0] == NULL)
    {
        return EXIT_SUCCESS;
    }

    // check for builtins first
    Command builtin;
    if (find_builtin(args[0], &builtin))
    {
        if (background)
        {
            fprintf(stderr, "Cannot start builtins in background.\n");
            return EXIT_FAILURE;
        }

        return builtin.function(argc, args);
    }

    // for (int i = 0; i < BUILTINS_COUNT; i++)
    // {
    //     if (strcmp(BUILTINS[i].name, args[0]) == 0)
    //     {
    //         if (background)
    //         {
    //             fprintf(stderr, "Cannot start builtins in background.\n");
    //             return EXIT_FAILURE;
    //         }

    //         return BUILTINS[i].function(argc, args);
    //     }
    // }

    return wsh_launch(argc, args, background);
}

void wsh_loop()
{
    if (init_signals() != 0)
    {
        fprintf(stderr, "Failed to init signals in wsh_loop().\n");
        exit(EXIT_FAILURE);
    }

    cmdtype_t type;
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

        if (argc == 0)
        {
            free(line);
            continue;
        }

        int split_index;

        if (wsh_get_command_type(argc, args, &split_index, &type) != 0)
        {
            free(line);
            free(args);
            continue;
        }

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

        case CMD_BACKGROUND:
            args[argc - 1] = NULL;
            argc--;
            code = wsh_execute(argc, args, true);
            break;

        default:
        case CMD_SIMPLE:
            code = wsh_execute(argc, args, false);
            break;
        }

        if (type != CMD_BACKGROUND)
        {
            printf(WSH_JOB_EXIT, code);
        }

        free(line);
        free(args);
    }
}