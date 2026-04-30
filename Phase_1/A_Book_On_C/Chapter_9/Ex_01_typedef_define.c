/*
 * File:    Ex_01_typedef_define.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This file demonstrates the use of `#define` to create a type alias
 *   for `float` as `DOLLARS`, calculating and displaying a simple
 *   interest calculation on a monetary amount. It shows an alternative
 *   to using `typedef` (which is commented out) for creating custom
 *   type names in C.
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
