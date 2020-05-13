#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "helpers.h"
#include "tree.h"
#include "hash.h"

#define MAX_STEPS 199

void printSolution(Branch *solution)
{
    Branch *steps[solution->depth + 1];
    Branch *current = solution;

    const char *directions[] = {
        "Up",
        "Right",
        "Down",
        "Left",
    };

    for (int i = solution->depth; i >= 0; i--)
    {
        steps[i] = current;
        current = current->root;
    }

    printf("Initial state :\n");
    printTab(steps[0]->value, SIZE_X, SIZE_Y);

    for (int i = 1; i <= solution->depth; i++)
    {
        printf("\n");
        printf("Move %s\n", directions[steps[i]->direction]);
        printTab(steps[i]->value, SIZE_X, SIZE_Y);
    }

    printf("Solution in %d steps\n", solution->depth);
}

void solve(Branch *branch, HashTable alreadyDone, int *target, bool *finished)
{
    if (branch->depth > MAX_STEPS)
    {
        // printf("%d\n", branch->depth);
        return;
    }

    if (arrayAreSame(branch->value, target, TAB_SIZE))
    {
        printSolution(branch);
        *finished = true;
        return;
    }

    Branch **successors = generateSuccessors(branch);

    for (int i = 0; i < MAX_SUCCESSORS; i++)
    {
        if (successors[i] != NULL && !isInTable(alreadyDone, successors[i]->value))
        {
            addToTable(alreadyDone, successors[i]->value);
            branch->successors[i] = successors[i];
        }
    }

    if (countSuccessors(branch) != 0)
    {
        /**
         * @TODO
         * Problem here: we call the solve function synchronously, looping around
         * the directions always in the same order (Up, Right, Down, Left) so in
         * this example, the Up direction is prioritized, leading to unoptimal
         * solutions. Still works if you just need A solution.
         **/
        for (int i = 0; i < MAX_SUCCESSORS; i++)
        {
            if (branch->successors[i] != NULL)
            {
                solve(branch->successors[i], alreadyDone, target, finished);
            }
        }
    }
    // else, no successors, branch dies
    return;
}

int main(void)
{
    int intialState[TAB_SIZE] = {
        5, 4, 0,
        6, 1, 8,
        7, 3, 2};

    int finalState[TAB_SIZE] = {
        // 5, 0, 4,
        // 6, 1, 8,
        // 7, 3, 2
        1, 2, 3,
        4, 5, 6,
        7, 8, 0};

    Branch *tree = createBranch();
    HashTable table = createHashTable();
    arraycpy(intialState, tree->value, TAB_SIZE);

    bool finished = false;
    solve(tree, table, finalState, &finished);

    if (!finished)
        printf("no solution\n");

    /* cleanup */
    free(tree);

    return 0;
}