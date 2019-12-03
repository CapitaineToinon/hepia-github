#define PLAYER_CHAR '#'
#define CPU_CHAR 'O'
#define NOONE_CHAR ' '

void print_board(int **p, int userRows, int userColumns);
void print_winner(int winner);
void help(const char *filename);