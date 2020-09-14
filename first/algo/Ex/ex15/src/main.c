#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "queue.h"

#define HASH_TABLE_SIZE 11

typedef Queue **HashTable;

HashTable createHashTable()
{
    HashTable table = (HashTable)malloc(sizeof(Queue *) * HASH_TABLE_SIZE);

    for (unsigned int i = 0; i < HASH_TABLE_SIZE; i++)
        table[i] = NULL;

    return table;
}

int h(int value)
{
    return value % HASH_TABLE_SIZE;
}

bool isInTable(HashTable table, int value)
{
    return table[h(value)] != NULL;
}

void addToTable(HashTable table, int value)
{
    if (isInTable(table, value))
        return;

    int i = h(value);

    if (table[i] == NULL)
    {
        table[i] = createQueue();
    }

    insert(table[i], value);
}

void printTable(HashTable table)
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        printf("%d: ", i);

        if (table[i] == NULL || isEmpty(table[i]))
        {
            printf("NULL");
        }
        else
        {
            QueueElement *current = table[i]->head;

            while (current != NULL)
            {
                printf("[ %c ]->", (char)current->value + 0x60);
                current = current->next;
            }

            printf("NULL");
        }

        printf("\n");
    }
}

int main(void)
{
    char *example = "unexempledetable";
    int length = strlen(example);

    HashTable table = createHashTable();

    for (int i = 0; i < length; i++)
    {
        int value = (int)example[i] - 0x60;
        addToTable(table, value);
    }

    printTable(table);
    return 0;
}