/*
 * File:    Ex_01_typedef_define.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates the difference between using #define and
 *   typedef for creating type aliases by calculating simple interest
 *   on a dollar amount, with the typedef version commented out to show
 *   the alternative approach.
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
