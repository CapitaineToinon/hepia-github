#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 100

int main(void)
{
    FILE *f;
    char buffer[BUF_SIZE];

    if ((f = fopen("/dev/abcd", "r")) == NULL)
    {
        printf("Error!");
        exit(1);
    }

    int r = fread(buffer, sizeof(char), BUF_SIZE, f);
    printf("%s (read %d byte(s)).\n", buffer, r);
    return 0;
}