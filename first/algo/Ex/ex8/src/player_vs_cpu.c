/**
 * Author:  Antoine Sutter
 * Email:   antoinesutter@gmail.com
 * Github:  CapitaineToinon
 */ 
#include "connect_four.h"
#include "output.h"
#include "input.h"
#include "player_vs_cpu.h"

/**
 * Starts a new game player vs cpu.
 * The cpu can me smart or not (plays randomly or not)
 */ 
void player_vs_cpu(bool smart)
{
    int **board;
    bool cpu_smart = smart;
    bool player1turn = true;
    bool game_over = false;

    int winner = NOONE, columns = 7, rows = 6,
        p_column = 0, p_row = 0;

    board = new_board(rows, columns);
    init_random();

    do
    {
        
        if (player1turn)
        {
            print_board(board, rows, columns);
            // ask the player to chose a column, ask again if column is full
            do
            {
                p_column = get_player_column(1, columns);
                p_row = get_available_row(board, rows, columns, p_column); // returns -1 if the column is full
            } while (p_row == -1);

            player_play(board, p_row, p_column);
        }
        else
        {
            cpu_play(board, rows, columns, cpu_smart);
        }

        winner = who_won(board, rows, columns);
        game_over = is_game_over(board, rows, columns);
        player1turn = !player1turn;

    } while (winner == NOONE && game_over == false);

    print_winner(winner);
    print_board(board, rows, columns);
}