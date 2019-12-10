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
static bool play_winning_spot(int **board, int rows, int columns, int who_to_check, int who_to_play);
static bool play_winning_row(int **board, int rows, int columns, int who_to_check, int who_to_play);
static bool play_winning_column(int **board, int rows, int columns, int who_to_check, int who_to_play);
static bool play_winning_diagonal(int **board, int rows, int columns, int who_to_check, int who_to_play);

static bool play_winning_candidates(int **board, int rows, int columns, int row, int column, int *candidates, int who_to_check, int who_to_play);
static int are_3_out_of_4(int who, int *candidates, int length);

// helpers
static void fill_board(int **p, int N, int M);

/**
 * Creates a new board to played with
 */ 
int **new_board(int N, int M)
{
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
int get_available_row(int **board, int rows, int columns, int column)
{
    int row, candidate = -1;

    if (column > columns)
        return candidate;

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
 * Returns if the game is over yet or not
 */ 
bool is_game_over(int **board, int rows, int columns)
{
    int i;

    for (i = 0; i < columns; i++)
        if (get_available_row(board, rows, columns, i + 1) != -1)
            return false; // found a playable column, aka game is not nover

    return true; // no playable column found, aka game is over
}

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
        c_row = get_available_row(board, rows, columns, c_column);
    } while (c_row == -1);

    play(board, c_row, c_column, PLAYER2);
}

/**
 * CPU tries to prevent the player from winning if player can win on the next turn.
 * Plays randomly otherwise
 */
static void cpu_play_smart(int **board, int rows, int columns)
{
    // play winning move if possible and exit if it happened
    if (play_winning_spot(board, rows, columns, PLAYER2, PLAYER2))
        return;

    // counter the player if possible and exit if it happened
    if (play_winning_spot(board, rows, columns, PLAYER1, PLAYER2))
        return;

    // no smart play was possible that turn so play randomly instead
    cpu_play_stupid(board, rows, columns);
}

/**
 * Tries to play smart on either rows, columns or diagonals.
 * returns true if a smart play was done, false otherwise
 */
static bool play_winning_spot(int **board, int rows, int columns, int who_to_check, int who_to_play)
{
    if (play_winning_row(board, rows, columns, who_to_check, who_to_play))
        return true;

    if (play_winning_column(board, rows, columns, who_to_check, who_to_play))
        return true;

    if (play_winning_diagonal(board, rows, columns, who_to_check, who_to_play))
        return true;

    // couldn't play
    return false;
}

/**
 * Checks rows for a smart move.
 * returns true if a smart play was done, false otherwise
 */
static bool play_winning_row(int **board, int rows, int columns, int who_to_check, int who_to_play)
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

            if (play_winning_candidates(board, rows, columns, i + 1, j + 1, candidates, who_to_check, who_to_play))
                return true;
        }
    }

    return false; // no rows found
}

/**
 * Checks columns for a smart move.
 * returns true if a smart play was done, false otherwise
 */
static bool play_winning_column(int **board, int rows, int columns, int who_to_check, int who_to_play)
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

            if (play_winning_candidates(board, rows, columns, j + 1, i + 1, candidates, who_to_check, who_to_play))
                return true;
        }
    }

    return false; // no column found
}

/**
 * Checks diagonals for a smart move.
 * returns true if a smart play was done, false otherwise
 */
static bool play_winning_diagonal(int **board, int rows, int columns, int who_to_check, int who_to_play)
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

            if (play_winning_candidates(board, rows, columns, i + 1, j + 1, candidates, who_to_check, who_to_play))
                return true;

            // also check from top right to bottom left
            for (k = 0; k < CONNECT; k++)
                candidates[k] = board[ii - k][j + k];

            if (play_winning_candidates(board, rows, columns, i + 1, j + 1, candidates, who_to_check, who_to_play))
                return true;
        }
    }

    return false;
}

/**
 * Main smart function that will play in a spot to either win or counter a play.
 * Takes N candidates and check if N - 1 are the same as who_to_check and that the remaining one is NOONE
 * If such a spot if found, check if playing in that column would actually fill the line to get a winning or countering move
 * 
 * returns true if a smart play was done, false otherwise
 */
static bool play_winning_candidates(int **board, int rows, int columns, int row, int column, int *candidates, int who_to_check, int who_to_play)
{
    int winning_candidate, winning_col, winning_row, current_row;

    winning_candidate = are_3_out_of_4(who_to_check, candidates, CONNECT);
    winning_col = column + winning_candidate;
    winning_row = get_available_row(board, rows, columns, winning_col);
    current_row = row;

    if (winning_candidate != -1 && winning_row == current_row)
    {
        play(board, winning_row, winning_col, who_to_play);
        return true;
    }

    return false;
}

/**
 * Returns the index of an empty spot where someone could play to win
 * aka when 3 out of 4 candidates are the same and the 4th one is NOONE.
 * 
 * returns -1 if conditions aren't satisfied.
 */
static int are_3_out_of_4(int who, int *candidates, int length)
{
    int same = 0, noone = 0;
    int i, index;

    for (i = 0; i < length; i++)
    {
        if (candidates[i] == who)
        {
            same++;
        }

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

/**
 * Fills the board with NOONE
 */ 
static void fill_board(int **p, int N, int M)
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            p[i][j] = NOONE;
}