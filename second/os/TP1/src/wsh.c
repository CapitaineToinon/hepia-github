#include "wsh.h"

// declared in builtins.c
extern const Command BUILTINS[];
extern const int BUILTINS_COUNT;

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
        cmdtype_t type = get_command_type(argc, args, &split_index);

        switch (type)
        {
        case CMD_PIPE:
            printf("pipe\n");
            code = process_pipe(argc, args, split_index);
            break;

        case CMD_FILEOUT:
            code = process_fileout(argc, args, split_index, false);
            break;

        case CMD_FILEAPPEND:
            code = process_fileout(argc, args, split_index, true);
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