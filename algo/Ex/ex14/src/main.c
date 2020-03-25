#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "helpers.h"

/**
 * This function will:
 *  1. generate the successors of the head;
 *  2. insert the successors to the queue;
 *  3. remove the head from the queue.
 * 
 * The function won't do anything if the queue is empty.
 */
void findSuccessors(Queue *queue)
{
    /* exit early if queue is empty */
    if (isEmpty(queue))
        return;

    /* get the head but don't remove it yet */
    int *tab = first(queue);

    /* size of the matrice */
    int SIZE_X = 3;
    int SIZE_Y = 3;

    /* find the 0 */
    for (int i = 0; i < TAB_SIZE; i++)
    {
        if (tab[i] == 0) /* found it, generate the successors */
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
                if (canDirection[dir])
                {
                    /* create the new matrice */
                    int *newMatrice = (int *)malloc(sizeof(int) * TAB_SIZE);
                    arraycpy(tab, newMatrice, TAB_SIZE);

                    /* make the point move in the direction */
                    arrayswap(newMatrice, i, directionSwapWith[dir]);

                    /* insert the new matrice */
                    insert(queue, newMatrice);
                }
            }

            break;
        }
    }

    /* remove the head */
    extract(queue);
}

void printQueue(QueueElement *head)
{
    if (head != NULL)
    {
        printArray(head->tab, TAB_SIZE);

        if (head->next != NULL)
            printQueue(head->next);
    }
}

int main(void)
{
    int tab[TAB_SIZE] = {1, 2, 3, 4, 0, 5, 6, 7, 8};
    int tab2[TAB_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    Queue *queue;

    /* first example */
    queue = createQueue();
    insert(queue, tab);

    printf("----- Base array -----\n");
    printArray(first(queue), TAB_SIZE);
    findSuccessors(queue);

    printf("Successors length: %d\n", depth(queue));
    printf("----- successors -----\n");
    printQueue(queue->head);
    free(queue);

    /* second example */
    queue = createQueue();
    insert(queue, tab2);

    printf("----- Base array -----\n");
    printArray(first(queue), TAB_SIZE);
    findSuccessors(queue);
    
    printf("Successors length: %d\n", depth(queue));
    printf("----- successors -----\n");
    printQueue(queue->head);
    free(queue);

    return 0;
}