#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sub(char (*r)[5][81])
{
    printf("%s\n", (*r)[2]);
}

int main(void)
{
    char r[5][81];
    strncpy(r[2], "Test.", 80);
    sub(&r);
    return 0;
}