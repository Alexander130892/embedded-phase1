/*
 * File:    pgm1.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This file implements a simple program that prints its own name
 *   followed by all command-line arguments passed to it, separated by
 *   spaces and ending with a newline.
 */



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>


#define  N 13
int fork(void);

int32_t main(int argc,char** argv)
{
    int i;

    printf("%s: ", argv[0]);
    for (i = 1; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    putchar('\n');
    return EXIT_SUCCESS;
}