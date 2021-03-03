#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void replace(char *target, char c)
{
    if (target == NULL)
        return;

    int len = strlen(target);

    if (len == 0)
        return;

    for (int i = 0; i < len; i++)
        if (target[i] == ' ')
            target[i] = c;
}

int main()
{
    char s[] = "Hello my name is Antoine";
    replace((char *)&s, '*');
    printf("%s\n", s);
}