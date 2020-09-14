#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

struct T_element
{
    char ch;                // stack containing chars
    struct T_element *next; // next element
};

typedef struct T_element Stack;

Stack *createStack(void);
void push(Stack *stack, char next);
void pop(Stack *stack);
Stack *top(Stack *stack);
bool isEmpty(Stack *stack);
int depth(Stack *stack);
