#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    char s[] = "-1";
    unsigned n;

    sscanf(s, "%u", &n);
    printf("Value of n: %u\n", n);
    return 0;
}
