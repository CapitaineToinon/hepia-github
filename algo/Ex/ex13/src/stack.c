#include "stack.h"

Stack *createStack(void)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->next = NULL;
    return stack;
}

void push(Stack *stack, char next)
{
    Stack *newTop = createStack();
    newTop->ch = next;
    top(stack)->next = newTop;
}

void pop(Stack *stack)
{
    if (!isEmpty(stack))
    {
        Stack *current = stack;
        Stack *next = stack->next;

        while (next->next != NULL)
        {
            current = next;
            next = current->next;
        }

        free(next);
        current->next = NULL;
    }
}

Stack *top(Stack *stack)
{
    Stack *last = stack;

    if (last->next != NULL)
    {
        last = top(last->next);
    }

    return last;
}

bool isEmpty(Stack *stack)
{
    return depth(stack) == 0;
}

int depth(Stack *stack)
{
    int count = 0;
    Stack *current = stack;

    while (current->next != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}