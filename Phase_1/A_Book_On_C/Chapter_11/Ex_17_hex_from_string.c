#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    char s[] = "0xabc";
    int n;

    sscanf(s, "%x", &n);
    printf("Value of n: %d\n", n);
    return 0;
}
