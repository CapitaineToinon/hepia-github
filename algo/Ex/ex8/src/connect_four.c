#include "connect_four.h"

// game over functions
static bool has_won(int **board, int rows, int columns, int who);
static bool are_same(int who, int *candidates, int length);
static bool check_rows(int **board, int rows, int columns, int who);
static bool check_columns(int **board, int rows, int columns, int who);
static bool check_diagonals(int **board, int rows, int columns, int who);

// cpu functions
static void cpu_play_stupid(int **board, int rows, int columns);
static void cpu_play_smart(int **board, int rows, int columns);
static int is_winning_move(int who, int *candidates, int length);

// helpers
static void fill_board(int **p, int N, int M);

// function based from : https://stackoverflow.com/questions/45397460/returning-multidimensional-arrays-from-a-function-in-c
// We return the pointer
int **new_board(int N, int M) /* Allocate the array */
{
    /* Check if allocation succeeded. (check for NULL pointer) */
    int i, **array;
    array = malloc(N * sizeof(int *));
    for (i = 0; i < N; i++)
        array[i] = malloc(M * sizeof(int));

    fill_board(array, N, M);
    return array;
}

/**
 * Returns who won the game
 */
int who_won(int **board, int rows, int columns)
{
    if (has_won(board, rows, columns, PLAYER1))
    {
        // player won, return now
        return PLAYER1;
    }

    if (has_won(board, rows, columns, PLAYER2))
    {
        // cpu won, return now
        return PLAYER2;
    }

    return NOONE;
}

/**
 * Returns the first avaiable row for a column.
 * Returns -1 if the column is already full
 */
int get_available_row(int **board, int rows, int column)
{
    int row, candidate = -1;

    for (row = rows - 1; row >= 0; row--)
    {
        if (board[row][column - 1] == NOONE)
        {
            candidate = row + 1;
            break;
        }
    }

    return candidate;
}

/**
 * Play
 */
void play(int **board, int row, int column, int who)
{
    board[row - 1][column - 1] = who;
}

/**
 * Plays for the player
 */
void player_play(int **board, int row, int column)
{
    play(board, row, column, PLAYER1);
}

/**
 * Plays for the CPU.
 * The CPU can be stupid (plays randomly) or smart (tries to prevent the player from winning)
 */
void cpu_play(int **board, int row, int column, bool smart)
{
    if (smart)
        cpu_play_smart(board, row, column);
    else
        cpu_play_stupid(board, row, column);
}

/**
 * Init the seed for rng. Only call this once!
 */
void init_random()
{
    srand((unsigned int)time(NULL)); /* Initialization, should only be called once. */
}

/****************************************************
 * GAME OVER FUNCTIONS
 ***************************************************/

/**
 * Returns true if a specific candidate has won the game
 */
static bool has_won(int **board, int rows, int columns, int who)
{
    return (check_rows(board, rows, columns, who) || check_columns(board, rows, columns, who) || check_diagonals(board, rows, columns, who));
}

/**
 * Returns true if candidates are the same
 */
static bool are_same(int who, int *candidates, int length)
{
    int i;
    for (i = 0; i < length; i++)
        if (candidates[i] != who)
            return false;

    return true;
}

/**
 * Returns true candidates are aligned on a row
 */
static bool check_rows(int **board, int rows, int columns, int who)
{
    int i, j, k;
    int *candidates = malloc(CONNECT * sizeof(int *));

    // swap columns and rows in the 2 fors compared to check_columns
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j <= columns - CONNECT; j++)
        {
            for (k = 0; k < CONNECT; k++)
                candidates[k] = board[i][j + k]; // swap j + k with i compared to check_columns

            if (are_same(who, candidates, CONNECT))
                return true; // won!
        }
    }

    return false; // no rows found
}

/**
 * Returns true candidates are aligned on a column
 */
static bool check_columns(int **board, int rows, int columns, int who)
{
    int i, j, k;
    int *candidates = malloc(CONNECT * sizeof(int *));

    // swap columns and rows in the 2 fors compared to check_rows
    for (i = 0; i < columns; i++)
    {
        for (j = 0; j <= rows - CONNECT; j++)
        {
            for (k = 0; k < CONNECT; k++)
                candidates[k] = board[j + k][i]; // swap j + k with i compared to check_rows

            if (are_same(who, candidates, CONNECT))
                return true; // won!
        }
    }

    return false; // no column found
}

/**
 * Returns true candidates are aligned on a diagonal
 */
static bool check_diagonals(int **board, int rows, int columns, int who)
{
    int i, ii, j, k;
    int *candidates = malloc(CONNECT * sizeof(int *));

    for (i = 0, ii = rows - 1; i <= rows - CONNECT; i++, ii--)
    {
        for (j = 0; j <= columns - CONNECT; j++)
        {
            // check from top left to bottom right
            for (k = 0; k < CONNECT; k++)
                candidates[k] = board[i + k][j + k];

            if (are_same(who, candidates, CONNECT))
                return true; // won!

            // also check from top right to bottom left
            for (k = 0; k < CONNECT; k++)
                candidates[k] = board[ii - k][j + k];

            if (are_same(who, candidates, CONNECT))
                return true; // won!
        }
    }

    return false;
}

/****************************************************
 * CPU FUNCTIONS
 ***************************************************/

/**
 * CPU plays randomly
 */
static void cpu_play_stupid(int **board, int rows, int columns)
{
    int c_column, c_row;
    // get the cpu to chose a column
    do
    {
        c_column = 1 + (rand() % columns);
        c_row = get_available_row(board, rows, c_column);
    } while (c_row == -1);

    play(board, c_row, c_column, PLAYER2);
}

/**
 * CPU tries to prevent the player from winning if player can win on the next turn.
 * Plays randomly otherwise
 */
static void cpu_play_smart(int **board, int rows, int columns)
{
    // todo
    cpu_play_stupid(board, rows, columns);
}

/**
 * Returns the index of an empty spot where someone could play to win
 * aka when 3 out of 4 candidates are the same and the 4th one is NOONE.
 * 
 * returns -1 if conditions aren't satisfied.
 */
static int is_winning_move(int who, int *candidates, int length)
{
    int same = 0, noone = 0;
    int i, index;

    for (i = 0; i < length; i++)
    {
        if (candidates[i] == who)
            same++;

        if (candidates[i] == NOONE)
        {
            noone++;
            index = i;
        }
    }

    if (same == length - 1 && noone == 1) 
        return index;
    else
        return -1;
}

/****************************************************
 * HELPERS FUNCTIONS
 ***************************************************/
static void fill_board(int **p, int N, int M)
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            p[i][j] = NOONE;
}