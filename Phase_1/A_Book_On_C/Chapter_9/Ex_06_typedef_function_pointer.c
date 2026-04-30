/*
 * File:    Ex_06_typedef_function_pointer.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates how to use typedef to create function
 *   pointer types in C by defining PFDD as a pointer to functions that
 *   take a double and return a double, then using it to call the sin
 *   and cos functions with PI as input.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define PI 3.14 //rows
#define COLLUMNS 5 // collumns

typedef double dbl;

//typedef dbl (*PFDD)(dbl);

typedef dbl (FDD)(dbl);
typedef FDD *PFDD;

//Typedefs
//typedef char* string;



int main(){
    PFDD f = sin, g = cos;
    printf("f(%.2f) = %.2f\n", PI, f(PI));
    printf("g(%.2f) = %.2f\n", PI, g(PI));
    return EXIT_SUCCESS;
}
