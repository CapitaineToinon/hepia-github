#include <math.h>
#include "queue.h"
#include "helpers.h"

#define HASH_TABLE_SIZE 10000

typedef Queue **HashTable;

HashTable createHashTable();
int h(int *tab);
bool isInTable(HashTable table, int *tab);
void addToTable(HashTable table, int *value);
int tableLength(HashTable table);