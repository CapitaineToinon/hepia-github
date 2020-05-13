#include "tree.h"
#include "helpers.h"

/* creates a branch */
Branch *createBranch()
{
    Branch *branch = (Branch *)malloc(sizeof(Branch));
    branch->value = (int *)malloc(sizeof(int) * TAB_SIZE);
    branch->depth = 0;
    branch->root = NULL;
    branch->successors = (Branch **)malloc(sizeof(Branch *) * MAX_SUCCESSORS);
    for (int i = 0; i < MAX_SUCCESSORS; i++)
        branch->successors[i] = NULL;
    return branch;
}

/* counts how many successors a branch has */
int countSuccessors(Branch *branch)
{
    int count = 0;

    for (int i = 0; i < MAX_SUCCESSORS; i++)
        if (branch->successors[i] != NULL)
            count++;

    return count;
}

int findSuccessorIndex(Branch *branch, int *search)
{
    for (int i = 0; i < MAX_SUCCESSORS; i++)
        if (branch->successors[i] != NULL)
            if (arrayAreSame(branch->successors[i]->value, search, TAB_SIZE))
                return i;

    return -1;
}

/* returns if a value if a successor of a branch or not */
bool isSuccessors(Branch *branch, int *search)
{
    return findSuccessorIndex(branch, search) != -1;
}

/**
 * Generates the sucessors of a branch but doesn't add them to the successors property
 */
Branch **generateSuccessors(Branch *branch)
{
    Branch **successors = (Branch **)malloc(sizeof(Branch *) * MAX_SUCCESSORS);

    /* find the 0 */
    for (int i = 0; i < TAB_SIZE; i++)
    {
        if (branch->value[i] == 0) /* found it, generate the successors */
        {
            int x = i % SIZE_X;
            int y = i / SIZE_Y;
            const int AMOUNT_DIRECTIONS = 4;

            /* whether the point can move in a direction or not */
            bool canDirection[AMOUNT_DIRECTIONS] = {
                y > 0,              // if it can go up
                (x < (SIZE_X - 1)), // if it can go right
                (y < (SIZE_Y - 1)), // if it can go down
                x > 0,              // if it can go left
            };

            /* the index of the point it needs to be swapped with */
            int directionSwapWith[AMOUNT_DIRECTIONS] = {
                i - SIZE_Y, // the index of the point to swap with if it can go up
                i + 1,      // the index of the point to swap with if it can go right
                i + SIZE_Y, // the index of the point to swap with if it can go down
                i - 1,      // the index of the point to swap with if it can go left
            };

            for (int dir = 0; dir < AMOUNT_DIRECTIONS; dir++)
            {
                Branch *newsuccessor = NULL;

                if (canDirection[dir])
                {
                    /* create the new matrice */
                    int *newMatrice = (int *)malloc(sizeof(int) * TAB_SIZE);
                    arraycpy(branch->value, newMatrice, TAB_SIZE);

                    /* make the point move in the direction */
                    arrayswap(newMatrice, i, directionSwapWith[dir]);

                    /* insert the new matrice */
                    newsuccessor = createBranch();
                    newsuccessor->root = branch;
                    newsuccessor->depth = branch->depth + 1;
                    newsuccessor->direction = (enum Direction)dir;
                    arraycpy(newMatrice, newsuccessor->value, TAB_SIZE);
                }

                successors[dir] = newsuccessor;
            }

            break;
        }
    }

    return successors;
}