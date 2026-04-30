/*
 * File:    Ex_01_typedef_define.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the use of `#define` preprocessor directive
 *   to create an alias for the `float` data type, calculating and
 *   displaying the sum of a monetary amount plus its interest. The
 *   file shows an alternative approach using `#define DOLLARS float`
 *   instead of the commented-out `typedef float DOLLARS;` declaration
 *   to achieve the same aliasing functionality.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define ACC 100 // number of fct calls
#define DOLLARS float

//Typedefs
//typedef float DOLLARS;



int main(){
    DOLLARS amount = 100.0;
    DOLLARS interest = 0.07 * amount;

    printf("DOLLARS = %.2f\n", amount+interest);
    return EXIT_SUCCESS;
}
