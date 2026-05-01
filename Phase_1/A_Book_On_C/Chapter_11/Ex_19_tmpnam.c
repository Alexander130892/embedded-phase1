/*
 * File:    Ex_19_tmpnam.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates the `tmpnam()` function by generating and
 *   printing three unique temporary filenames to show how the function
 *   creates different temporary file names on each call.
 */
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
