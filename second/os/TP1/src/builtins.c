#include "builtins.h"
#include "common.h"

const Command BUILTINS[] = {
    {"help", wsh_help},
    {"exit", wsh_exit},
    {"cd", wsh_cd},
    {"type", wsh_type},
};

const int BUILTINS_COUNT = sizeof(BUILTINS) / sizeof(Command);

int wsh_help(int argc, char **args)
{
    (void)argc;
    (void)args;
    printf("This is the weeb shell you stupid fuck\n");
    return EXIT_SUCCESS;
}

int wsh_exit(int argc, char **args)
{
    (void)argc;
    (void)args;
    printf(WSH_EXIT_MESSAGE);
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

    for (int i = 0; i < BUILTINS_COUNT; i++)
    {
        if (strcmp(BUILTINS[i].name, args[1]) == 0)
        {
            printf("%s is a shell builtin\n", args[1]);
            return EXIT_SUCCESS;
        }
    }

    printf("%s is not a shell builtin\n", args[1]);
    return EXIT_SUCCESS;
}