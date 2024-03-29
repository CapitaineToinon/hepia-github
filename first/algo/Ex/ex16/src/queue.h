#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#define SIZE_X 3
#define SIZE_Y 3
#define TAB_SIZE (SIZE_X * SIZE_Y)

struct T_queue_element
{
    void *value;
    struct T_queue_element *next;
};

typedef struct T_queue_element QueueElement;

struct T_queue
{
    QueueElement *head;
    QueueElement *tail;
    int length; // nbre elements de la file
};

typedef struct T_queue Queue;

Queue *createQueue();
void insert(Queue *queue, void *value);
void extract(Queue *queue);
void *first(Queue *queue);
int depth(Queue *queue);
bool isEmpty(Queue *queue);
