/*
 * File:    pgm1.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program prints the program name followed by all
 *   command-line arguments passed to it, then exits successfully.
 *   Despite defining N=13 and declaring fork(), the main function only
 *   performs argument echoing without using these declarations.
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