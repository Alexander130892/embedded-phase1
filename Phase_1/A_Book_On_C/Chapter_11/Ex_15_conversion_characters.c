#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    int a, b, c;
    printf("a%nb%nc%n", &a, &b, &c);   // %n writes happen here
    printf(" %d %d %d\n", a, b, c);    // now a,b,c are initialized
    return 0;
}
