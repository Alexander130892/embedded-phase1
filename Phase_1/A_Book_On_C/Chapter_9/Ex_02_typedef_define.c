/*
 * File:    Ex_02_typedef_define.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the difference between `#define` and
 *   `typedef` by showing how `typedef` allows creating a new type name
 *   (DOLLARS) that can be safely redeclared as a variable in an inner
 *   scope, while `#define` would cause a compilation error in the same
 *   situation.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define ACC 100 // number of fct calls
//#define DOLLARS float

//Typedefs
typedef float DOLLARS;



int main(){
    DOLLARS amount = 100.0;
    DOLLARS interest = 0.07 * amount;
    {
        float DOLLARS;
        DOLLARS = amount + interest;
        printf("DOLLARS = %.2f\n", DOLLARS); // NOK with #define
    }
    
    return EXIT_SUCCESS;
}
