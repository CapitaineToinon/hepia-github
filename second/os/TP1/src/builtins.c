#include "builtins.h"

const Command BUILTINS[] = {
    {"help", wsh_help},
    {"exit", wsh_exit},
    {"cd", wsh_cd},
    {"type", wsh_type},
    {"hi", wsh_hi},
};

const int BUILTINS_COUNT = sizeof(BUILTINS) / sizeof(Command);

bool find_builtin(char *name, Command *command)
{
    if (name == NULL)
    {
        return false;
    }

    for (int i = 0; i < BUILTINS_COUNT; i++)
    {
        if (strncmp(BUILTINS[i].name, name, PATH_MAX) == 0)
        {
            *command = BUILTINS[i];
            return true;
        }
    }

    // builtin was not found
    return false;
}

bool is_builtin(char *name)
{
    return find_builtin(name, NULL);
}

int wsh_help(int argc, char **args)
{
    // unused params
    (void)argc;
    (void)args;
    printf("This is the weeb shell you stupid fuck\n");
    return EXIT_SUCCESS;
}

int wsh_exit(int argc, char **args)
{
    // unused params
    (void)argc;
    (void)args;
    printf(MAGENTA WSH_EXIT_MESSAGE RESET);
    exit(EXIT_SUCCESS);
}

int wsh_cd(int argc, char **args)
{
    if (argc != 2)
    {
        fprintf(stderr, "%s: cd expected exactly one argument\n", WSH_SHELL_NAME);
        return EXIT_FAILURE;
    }

    if (chdir(args[1]) != 0)
    {
        perror(WSH_SHELL_NAME);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int wsh_type(int argc, char **args)
{
    if (argc < 2)
    {
        fprintf(stderr, "%s: type expected one argument\n", WSH_SHELL_NAME);
        return EXIT_FAILURE;
    }

    if (is_builtin(args[1]))
    {
        printf("%s is a shell builtin\n", args[1]);
    }
    else
    {
        printf("%s is not a shell builtin\n", args[1]);
    }

    return EXIT_SUCCESS;
}

int wsh_hi(int argc, char **args)
{
    // unused params
    (void)argc;
    (void)args;
    printf("Hi! Welcome to the weeb shell (wsh).\n");
    return EXIT_SUCCESS;
}