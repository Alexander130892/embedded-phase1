/*
 * File:    Ex_03_ptrdiff.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates the use of the ptrdiff_t data type by
 *   calculating and printing the pointer differences between two
 *   integer pointers and between a pointer and a null pointer.
 */
 //Libraries
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define COUNT 1000000.0

//Typedefs

//Prototypes


int main() {
    int a,b,*p=&a, *q=&b;
    ptrdiff_t diff = p-q;
    printf("diff = %ld\n", diff);
    diff = p-(int*)0;
    printf("diff = %ld\n", diff);
    return 0;
}