/**
 * Antoine Sutter
 * antoinesutter@gmail.com
 * https://www.github.com/CapitaineToinon
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "helpers.h"
#include "tree.h"
#include "hash.h"

#define MAX_STEPS 100

/**
 * Prints the solution on the screen
 */
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

/**
 * Solves the puzzle only up to a maximum depths.
 * This function will call itself recusrively up to maxdepths
 */
Branch *recursive(Branch *branch, HashTable alreadyDone, int *target, int maxdepths)
{
    // The value of the leaf is the solution we're looking for, exit !
    if (arrayAreSame(branch->value, target, TAB_SIZE))
    {
        return branch;
    }

    // We reach the maximum depth we're willing to go, exit now
    if (branch->depth > maxdepths)
    {
        return NULL;
    }

    // If we reach this point, then the current branch isn't the solution.
    // Only generate successors if the branch doesn't already have ones.
    if (countSuccessors(branch) <= 0)
    {
        // Generate the possible successors and call this function on each
        // succerror recusrively.
        Branch **successors = generateSuccessors(branch); // Up to 4 successors

        for (int i = 0; i < MAX_SUCCESSORS; i++)
        {
            // We're also using a Hash table to avoid
            // visiting patterns we've already visited before.
            if (successors[i] != NULL && !isInTable(alreadyDone, successors[i]->value))
            {
                addToTable(alreadyDone, successors[i]->value);
                branch->successors[i] = successors[i];
            }
        }
    }

    // Call this function recusrively on each successor.
    // If the branch didn't generate any successors, they
    // will all be NULL, thus making the branch die.
    for (int i = 0; i < MAX_SUCCESSORS; i++)
    {
        if (branch->successors[i] != NULL)
        {
            Branch *solution = recursive(branch->successors[i], alreadyDone, target, maxdepths);

            if (solution != NULL)
                return solution; // We found a solution in one of the successor !
        }
    }

    // If we reach this, then no successor was found
    return NULL;
}

/**
 * Solves the puzzle only up to a maximum depths.
 * Returns a pointer to the solution. 
 * Returns NULL if no solution was found.
 */
Branch *solve(int *intialState, int *target)
{
    int i;
    Branch *solution = NULL;
    Branch *tree = createBranch();
    HashTable alreadyDone = createHashTable(); // keeps track of the already visited nodes
    arraycpy(intialState, tree->value, TAB_SIZE);

    // Progressively increased the allowed max deaths up to MAX_STEPS
    // If there are no solution in 1 move, then try
    // to find one is 2 moves, then 3 moves, etc...
    // Doing this guarantees the solution is in the least
    // amount of moves possibles. There may be other solution(s)
    // in the same amount of moves though.
    for (i = 0; i < MAX_STEPS; i++)
    {
        solution = recursive(tree, alreadyDone, target, i);

        if (solution != NULL)
            break;
        // else, there are no solution in i steps
    }

    free(alreadyDone);

    // NULL if no solution
    return solution;
}

int main(void)
{
    int intialState[TAB_SIZE] = {
        5, 4, 0,
        6, 1, 8,
        7, 3, 2};

    int finalState[TAB_SIZE] = {
        1, 2, 3,
        4, 5, 6,
        7, 8, 0};

    Branch *solution = solve(intialState, finalState);

    if (solution == NULL)
        printf("No solution was found\n");
    else
        printSolution(solution);

    free(solution);

    return 0;
}