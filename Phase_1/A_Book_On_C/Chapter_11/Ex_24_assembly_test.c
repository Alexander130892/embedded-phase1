/*
 * File:    Ex_24_assembly_test.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file implements a simple countdown loop that prints
 *   decreasing integer values from 9 down to 1, likely created as a
 *   test case for examining assembly code generation or debugging
 *   assembly output.
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