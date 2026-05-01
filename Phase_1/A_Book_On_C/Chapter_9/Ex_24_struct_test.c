/*
 * File:    Ex_24_struct_test.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the behavior of bit fields in a C struct by
 *   assigning the same integer value to three unsigned bit fields of
 *   different widths (1, 2, and 3 bits) and printing how each field
 *   truncates the value based on its bit capacity.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
struct test {
    unsigned a:1, b:2, c:3;
};

int main(){
    int         i;
    struct test x;
    for (i=0; i < 23; i++)
    {
        x.a = x.b = x.c = i;
        //x.c = x.b = x.a = i;
        printf("x.a = %d    x.b = %d    x.c = %d\n", x.a, x.b, x.c);
    }
    return EXIT_SUCCESS;
}
