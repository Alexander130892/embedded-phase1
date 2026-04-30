#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    printf("%0+17d\n", 1);
    printf("%+017d\n", 1);
    return 0;
}
