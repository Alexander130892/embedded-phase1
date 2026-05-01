/*
 * File:    Ex_24_assembly_test.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements a simple countdown loop that prints the
 *   values of a variable `i` starting from 9 down to 1, demonstrating
 *   pre-decrement operation in a while loop condition.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>

#define MAXSTRING 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING 100

int main(int argc, char **argv)
{
    int i =10;
    while(--i)
        printf("i = %d\n", i);
    return EXIT_SUCCESS;
}