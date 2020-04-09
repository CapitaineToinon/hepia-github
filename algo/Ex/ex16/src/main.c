#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include "helpers.h"
#include "queue.h"
#include "tree.h"

#define HASH_TABLE_SIZE 10000

typedef Queue **HashTable;

HashTable createHashTable()
{
    HashTable table = (HashTable)malloc(sizeof(Queue *) * HASH_TABLE_SIZE);

    for (unsigned int i = 0; i < HASH_TABLE_SIZE; i++)
        table[i] = NULL;

    return table;
}

/* hash function */
int h(int *tab)
{
    int sum = 0;

    for (int i = 0; i < TAB_SIZE; i++)
        sum += pow(tab[i], i);

    return sum % HASH_TABLE_SIZE;
}

bool isInTable(HashTable table, int *tab)
{
    int i = h(tab);

    if (table[i] != NULL)
    {
        QueueElement *current = table[i]->head;

        while (current != NULL)
        {
            if (arrayAreSame((int *)current->value, tab, TAB_SIZE))
                return true;

            current = current->next;
        }
    }

    return false;
}

void addToTable(HashTable table, int *value)
{
    if (isInTable(table, value))
        return;

    int i = h(value);

    if (table[i] == NULL)
        table[i] = createQueue();

    insert(table[i], value);
}

int tableLength(HashTable table)
{
    int length = 0;

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        if (table[i] != NULL)
            length += depth(table[i]);

    return length;
}

void printQueue(QueueElement *head)
{
    if (head != NULL)
    {
        printTab((int *)head->value, SIZE_X, SIZE_Y);

        if (head->next != NULL)
            printQueue(head->next);
    }
}

void printSolution(Branch *solution)
{
    Branch *steps[solution->depth];
    Branch *current = solution;

    const char *directions[] = {
        "Up",
        "Right",
        "Down",
        "Left",
    };

    for (int i = solution->depth - 1; i >= 0; i--)
    {
        steps[i] = current;
        current = current->root;
    }

    printf("Initial state :\n");
    printTab(steps[0]->value, SIZE_X, SIZE_Y);
    printf("\n");

    for (int i = 1; i < solution->depth; i++)
    {
        printf("Move %s\n", directions[steps[i]->direction]);
        printTab(steps[i]->value, SIZE_X, SIZE_Y);
        printf("\n");
    }

    printf("Depths of the solution %d\n", solution->depth);
}

void solve(Branch *branch, HashTable alreadyDone, int *target, bool *finished)
{
    if (*(finished) == true)
        return;

    Branch **successors = generateSuccessors(branch);

    for (int i = 0; i < MAX_SUCCESSORS; i++)
    {
        if (successors[i] != NULL && !isInTable(alreadyDone, successors[i]->value))
        {
            addToTable(alreadyDone, successors[i]->value);
            branch->successors[i] = successors[i];
        }
    }

    if (isSuccessors(branch, target))
    {
        printSolution(branch->successors[findSuccessorIndex(branch, target)]);
        (*finished) = true;
    }
    else
    {
        if (countSuccessors(branch) != 0)
        {
            for (int i = 0; i < MAX_SUCCESSORS; i++)
            {
                if (branch->successors[i] != NULL)
                {
                    solve(branch->successors[i], alreadyDone, target, finished);
                }
            }
        }
        else
        {
            printf("Branch dies...\n");
        }
    }
}

int main(void)
{
    int intialState[TAB_SIZE] = {
        5, 4, 0,
        6, 1, 8,
        7, 3, 2};

    int finalState[TAB_SIZE] = {
        // 5, 4, 8,
        // 6, 1, 2,
        // 7, 3, 0
        // 5, 4, 8,
        // 6, 1, 2,
        // 7, 0, 3
        5, 1, 4,
        6, 0, 8,
        7, 3, 2
        // 1, 2, 3,
        // 4, 5, 6,
        // 7, 8, 0
    };

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