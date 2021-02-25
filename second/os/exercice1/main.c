#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

const char *NAME = "Antoine";

void otherFunction()
{
    printf("This is a function\n");
    return;
}

int main()
{
    pid_t pid = getpid();
    int age = 25;
    int *count = (int *)malloc(sizeof(int));

    printf("My PID = %d\n", pid);
    printf("Address of global variable 'NAME': %p\n", &NAME);
    printf("Address of local variable 'age': %p\n", &age);
    printf("Address of local variable allocated with malloc 'count': %p\n", &count);
    printf("Address of function 'otherFunction': %p\n", &otherFunction);
    printf("Address of function 'main': %p\n", &main);

    fgetc(stdin);
}