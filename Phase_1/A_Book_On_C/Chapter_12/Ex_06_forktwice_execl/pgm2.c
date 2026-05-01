/*
 * File:    pgm2.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program calculates and prints the sum of all valid integer
 *   command-line arguments passed to it, skipping any arguments that
 *   cannot be parsed as integers.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>


#define  N 13
int fork(void);

int32_t main(int argc,char** argv)
{
    int i, sum=0, value;

    for (i = 1; i < argc; i++)
    {
        if(sscanf(argv[i], "%d", &value) == 1){
            sum+=value;
        }
    }
    printf("%s: sum of command line args = %d\n", argv[0], sum);
    return EXIT_SUCCESS;
}