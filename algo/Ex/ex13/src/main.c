#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define MAX_STACK_LENGTH 50
#define END_CHAR '!'

void printWord(Stack *stack);
void reverseStack(Stack *word);
void swapTopAndPop(Stack *from, Stack *to);

int main(void)
{
    Stack *word = createStack();
    int c = '\n';

    printf("Type a word, character by character and end with \"%c\".\n", END_CHAR);

    for (;;)
    {
        c = getchar();
        // consume newline
        (void)getchar();

        if (c == '!')
        {
            break;
        }

        push(word, c);
        printf("Your word so far: ");
        printWord(word);
    }

    printf("Before  :");
    printWord(word);

    reverseStack(word);

    printf("After   :");
    printWord(word);

    free(word);

    return 0;
}

void printWord(Stack *stack)
{
    int i, count = depth(stack);
    Stack *currentLetter = stack;

    for (i = 0; i < count; i++)
    {
        printf("%c", currentLetter->next->ch);
        currentLetter = currentLetter->next;
    }

    printf("\n");
}

void reverseStack(Stack *word)
{
    Stack *reversed = createStack();

    while (!isEmpty(word))
    {
        swapTopAndPop(word, reversed);
    }

    free(word->next);
    word->next = reversed->next;
    free(reversed);
}

void swapTopAndPop(Stack *from, Stack *to)
{
    push(to, top(from)->ch);
    pop(from);
}