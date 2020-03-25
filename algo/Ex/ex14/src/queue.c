#include "queue.h"

QueueElement *_createQueueElement()
{
    QueueElement *element = (QueueElement *)malloc(sizeof(QueueElement));
    element->next = NULL;
    return element;
}

Queue *createQueue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void insert(Queue *queue, int tab[TAB_SIZE])
{
    QueueElement *newElement = _createQueueElement();

    for (int i = 0; i < TAB_SIZE; i++)
        newElement->tab[i] = tab[i];

    /**
     * If the tails isn't empty, chain it correctly
     */
    if (queue->tail != NULL)
    {
        queue->tail->next = newElement;
    }

    /**
     *  Add the new element as the tail
     */
    queue->tail = newElement;

    /** 
     * if the head is empty, then then queue is empty and 
     * newElement is both head and tail
     */
    if (queue->head == NULL)
    {
        queue->head = newElement;
    }

    /* increase the length */
    queue->length += 1;
}

bool extract(Queue *queue, int destination[TAB_SIZE])
{
    bool extracted = false;

    if (queue->head != NULL)
    {
        for (int i = 0; i < TAB_SIZE; i++)
            destination[i] = queue->head->tab[i];

        /* Replace the head if possible */
        if (queue->head->next != NULL)
        {
            QueueElement *newHead = queue->head->next;
            free(queue->head);
            queue->head = newHead;
        }
        else
        {
            free(queue->head);
            queue->head = NULL;
            queue->tail = NULL;
        }

        /* decrease length */
        queue->length -= 1;
        extracted = true;
    }

    return extracted;
}

QueueElement *first(Queue *queue)
{
    return queue->head;
}

int depth(Queue *queue)
{
    return queue->length;
}

bool isEmpty(Queue *queue)
{
    return depth(queue) <= 0;
}