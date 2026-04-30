#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    char tfn[100]; //tfn = temporary file name
    
    tmpnam(tfn);
    printf("1: tfn %s\n", tfn);
    tmpnam(tfn);
    printf("2: tfn %s\n", tfn);
    tmpnam(tfn);
    printf("3: tfn %s\n", tfn);
    return 0;
}
