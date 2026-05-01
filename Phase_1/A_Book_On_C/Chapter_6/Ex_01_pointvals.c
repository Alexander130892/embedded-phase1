/*
 * File:    Ex_01_pointvals.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates pointer arithmetic and dereferencing
 *   operations by printing various calculations involving a pointer to
 *   an integer, including the pointer's address, dereferenced values
 *   with arithmetic operations, and the difference between pointer
 *   addresses cast to integers.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define COUNT 1000000.0

//Typedefs

//Prototypes


int main() {
    char* format= "%p %d %d %d\n";
    int i = 3;
    int *p = &i;
    printf(format, p, *p+7, 3 * **&p +1, 5*(p-(p-2)));
    //ex 2
    printf("%d %d",(int) p, (int)(p-2));
    return 0;
}