/*
 * File:    Ex_02_typedef_define.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This file demonstrates the difference between `typedef` and
 *   `#define` by showing how `typedef` creates a true type alias that
 *   can be shadowed by local variables, while `#define` performs
 *   simple text substitution that would cause compilation errors in
 *   the same scenario.
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
