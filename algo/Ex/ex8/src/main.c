#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "output.h"
#include "player_vs_cpu.h"
#include "player_vs_player.h"

int main(int argc, char *argv[])
{
    int gamemode = -1;
    const char input[50];
    const char *filename = argv[0];

    if (argc <= 1)
    {
        help(filename);
        return -1;
    }
    
    strcpy((char *)input, argv[1]);
    gamemode = atoi(input);

    switch (gamemode) {
        case 1:
            player_vs_player();
            break;
        case 2:
            // player vs stupid cpu
            player_vs_cpu(false);
            break;
        case 3:
            // player vs smart cpu
            player_vs_cpu(true);
            break;
        default:
            help(filename);
            break;
    }

    return 0;
}

