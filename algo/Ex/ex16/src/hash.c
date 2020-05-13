#include "hash.h"

/**
 * Creates a hash table
 */
HashTable createHashTable()
{
    HashTable table = (HashTable)malloc(sizeof(Queue *) * HASH_TABLE_SIZE);

    for (unsigned int i = 0; i < HASH_TABLE_SIZE; i++)
        table[i] = NULL;

    return table;
}

/** 
 * hash function
 */
int h(int *tab)
{
    int sum = 0;

    for (int i = 0; i < TAB_SIZE; i++)
        sum += pow(tab[i], i);

    return sum % HASH_TABLE_SIZE;
}

/**
 * Return whether a value is in the hash table or not
 */
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

/**
 * Add a new value to a given hash table
 */
void addToTable(HashTable table, int *value)
{
    if (isInTable(table, value))
        return;

    int i = h(value);

    if (table[i] == NULL)
        table[i] = createQueue();

    insert(table[i], value);
}

/**
 * Returns the length of a given hash table
 */
int tableLength(HashTable table)
{
    int length = 0;

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        if (table[i] != NULL)
            length += depth(table[i]);

    return length;
}