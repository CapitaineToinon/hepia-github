#include <stdio.h>
#include <stdbool.h>
#include "connect_four.h"
#include "output.h"
#include "input.h"
#include "player_vs_cpu.h"

void player_vs_cpu(bool smart)
{
    int **board;
    bool cpu_smart = smart;
    int winner = NOONE, columns = 7, rows = 6,
        p_column = 0, p_row = 0;

    board = new_board(rows, columns);
    init_random();

    do
    {
        print_board(board, rows, columns);

        // ask the player to chose a column, ask again if column is full
        do
        {
            p_column = get_player_column(1, columns);
            p_row = get_available_row(board, rows, p_column); // returns -1 if the column is full
        } while (p_row == -1);

        player_play(board, p_row, p_column);
        winner = who_won(board, rows, columns);
        if (winner != NOONE)
            continue;

        cpu_play(board, rows, columns, cpu_smart);
        winner = who_won(board, rows, columns);
        if (winner != NOONE)
            continue;

    } while (winner == NOONE);

    print_winner(winner);
    print_board(board, rows, columns);
}