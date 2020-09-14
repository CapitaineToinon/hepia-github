/**
 * Author:  Antoine Sutter
 * Email:   antoinesutter@gmail.com
 * Github:  CapitaineToinon
 */ 
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NOONE 0
#define PLAYER1 1
#define PLAYER2 2
#define CONNECT 4

// public main functions
int **new_board(int N, int M);
bool is_game_over(int **board, int rows, int columns);
int who_won(int **board, int rows, int columns);
int get_available_row(int **board, int rows, int columns, int column);
void play(int **board, int row, int column, int who);
void player_play(int **board, int row, int column);
void cpu_play(int **board, int row, int column, bool smart);
void init_random();