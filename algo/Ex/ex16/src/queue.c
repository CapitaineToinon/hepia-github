#include "queue.h"

/**
 * Creates a queue element
 */
QueueElement *_createQueueElement()
{
    QueueElement *element = (QueueElement *)malloc(sizeof(QueueElement));
    element->next = NULL;
    return element;
}

/**
 * Creates a queue
 */
Queue *createQueue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

/**
 * Inserts an element to the queue
 */
void insert(Queue *queue, void *value)
{
    QueueElement *newElement = _createQueueElement();
    newElement->value = value;

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

/**
 * Remove last element in the queue
 */
void extract(Queue *queue)
{
    if (queue->head != NULL)
    {
        QueueElement *toRemove = queue->head;

        /* replace the head if possible */
        if (queue->head->next != NULL)
        {
            queue->head = queue->head->next;
        }
        else
        {
            queue->head = NULL;
            queue->tail = NULL;
        }

        /* decrease length */
        queue->length -= 1;

        free(toRemove);
    }
}

/**
 * Returns the first element in a given queue
 */
void *first(Queue *queue)
{
    return queue->head->value;
}

/**
 * Returns the depth of a given queue
 */
int depth(Queue *queue)
{
    return queue->length;
}

/**
 * Returns whether a given queue is empty or not
 */
bool isEmpty(Queue *queue)
{
    return depth(queue) <= 0;
}