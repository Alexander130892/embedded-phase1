/*
 * File:    Ex_26_print_test.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file compares the memory sizes of two similar bit-field
 *   structures to demonstrate how zero-width bit-field members affect
 *   structure padding and alignment. It prints the ratio of sizes
 *   between a structure with a zero-width bit-field separator and one
 *   without, showing the memory layout differences.
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
