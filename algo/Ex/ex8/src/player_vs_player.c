#include "connect_four.h"
#include "output.h"
#include "input.h"
#include "player_vs_player.h"

void player_vs_player()
{
    int **board;
    int winner = NOONE, i, player, player_count = 2,
        columns = 7, rows = 6,
        p_column = 0, p_row = 0;

    int players[2] = {PLAYER1, PLAYER2};

    board = new_board(rows, columns);
    init_random();

    do
    {
        // foreach player
        for (i = 0; i < player_count; i++)
        {
            player = players[i];
            print_board(board, rows, columns);

            // ask the player to chose a column, ask again if column is full
            do
            {
                printf("Player %d:\n", player);
                p_column = get_player_column(1, columns);
                p_row = get_available_row(board, rows, p_column); // returns -1 if the column is full
            } while (p_row == -1);

            play(board, p_row, p_column, player);
            winner = who_won(board, rows, columns);
            if (winner != NOONE)
                break;
        }

    } while (winner == NOONE);

    print_winner(winner);
    print_board(board, rows, columns);
}