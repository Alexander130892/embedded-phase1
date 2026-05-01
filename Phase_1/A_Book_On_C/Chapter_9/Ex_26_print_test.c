/*
 * File:    Ex_26_print_test.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file tests the behavior of bit-field alignment in C
 *   structures by comparing the size of a structure with an anonymous
 *   zero-width bit-field (which forces alignment) to one without,
 *   printing the ratio as a floating-point value.
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
