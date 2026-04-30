/*
 * File:    Ex_26_print_test.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This file compares the memory sizes of two bit-field structures
 *   where one uses a zero-width bit-field separator and the other
 *   doesn't, printing the ratio of their sizes as a floating-point
 *   number. It demonstrates how zero-width bit-fields force alignment
 *   to the next storage unit boundary, affecting structure size.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts


int main(){
    typedef struct{unsigned int a : 1, :0, b:1;}a;
    typedef struct{unsigned int a : 1, b:1;}b;
    printf("%.1f\n", (float) sizeof(a)/sizeof(b));
    return EXIT_SUCCESS;
}
