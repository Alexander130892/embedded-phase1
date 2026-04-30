/*
 * File:    Ex_01_pointvals.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates pointer arithmetic and dereferencing
 *   operations in C by printing the address of an integer variable and
 *   the results of various pointer-based calculations. It also casts
 *   pointer addresses to integers to show their numeric values.
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