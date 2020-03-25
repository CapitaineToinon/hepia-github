#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#define TAB_SIZE 9

struct T_queue_element
{
    int tab[TAB_SIZE];
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
void insert(Queue *queue, int tab[TAB_SIZE]);
void extract(Queue *queue);
int *first(Queue *queue);
int depth(Queue *queue);
bool isEmpty(Queue *queue);
