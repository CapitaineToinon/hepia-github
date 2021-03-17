#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#define MAX_STRING_SIZE 256

// Recursive
void strtoupper(char *string, char *output)
{
    int index;
    for (index = 0; string[index]; index++)
    {
        output[index] = toupper(string[index]);
    }
    output[index] = 0;
}

int main()
{   
    char input[] = "Hello";
    char output[strlen(input)];
    strtoupper(input, output);
    printf("%s\n", output);
    return 0;
}