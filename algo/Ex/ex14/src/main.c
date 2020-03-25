#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "helpers.h"

/**
 * This function will
 *  1. generate the successors of the head
 *  2. insert the successors to the queue
 *  3. remove the head from the queue
 */
void findSuccessors(Queue *queue)
{
    /* exit early if queue is empty */
    if (isEmpty(queue))
        return;

    /* get the head but don't remove it yet */
    int *tab = (int *)malloc(sizeof(int) * TAB_SIZE);
    tab = first(queue);

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

            /* could it go up ? */
            if (y > 0)
            {
                int *up = (int *)malloc(sizeof(int) * TAB_SIZE);
                arraycpy(tab, up, TAB_SIZE);

                int j = i - SIZE_Y;
                arrayswap(up, i, j);

                insert(queue, up);
            }

            /* could it go right ? */
            if (x < SIZE_X - 1)
            {
                int *right = (int *)malloc(sizeof(int) * TAB_SIZE);
                arraycpy(tab, right, TAB_SIZE);

                int j = i + 1;
                arrayswap(right, i, j);

                insert(queue, right);
            }

            /* could it go down ? */
            if (y < SIZE_Y - 1)
            {
                int *down = (int *)malloc(sizeof(int) * TAB_SIZE);
                arraycpy(tab, down, TAB_SIZE);

                int j = i + SIZE_Y;
                arrayswap(down, i, j);

                insert(queue, down);
            }

            /* could it go left ? */
            if (x > 0)
            {
                int *left = (int *)malloc(sizeof(int) * TAB_SIZE);
                arraycpy(tab, left, TAB_SIZE);

                int j = i - 1;
                arrayswap(left, i, j);

                insert(queue, left);
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
    findSuccessors(queue);

    printf("----- Base array -----\n");
    printArray(first(queue), TAB_SIZE);
    printf("Successors length: %d\n", depth(queue));
    printf("----- successors -----\n");
    printQueue(queue->head);
    free(queue);

    /* second example */
    queue = createQueue();
    insert(queue, tab2);
    findSuccessors(queue);

    printf("----- Base array -----\n");
    printArray(first(queue), TAB_SIZE);
    printf("Successors length: %d\n", depth(queue));
    printf("----- successors -----\n");
    printQueue(queue->head);
    free(queue);

    return 0;
}