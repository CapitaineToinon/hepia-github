#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define SIZE_X 3
#define SIZE_Y 3
#define TAB_SIZE (SIZE_X * SIZE_Y)
#define MAX_SUCCESSORS 4

enum Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3,
};

struct T_branch
{
    int *value;
    int depth;
    enum Direction direction;
    struct T_branch *root;
    struct T_branch **successors;
};

typedef struct T_branch Branch;

Branch *createBranch();
int countSuccessors(Branch *branch);
int findSuccessorIndex(Branch *branch, int *search);
bool isSuccessors(Branch *branch, int *search);
Branch **generateSuccessors(Branch *branch);