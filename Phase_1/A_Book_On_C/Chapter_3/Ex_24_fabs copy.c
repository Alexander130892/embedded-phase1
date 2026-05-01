/*
 * File:    Ex_24_fabs copy.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates the difference between the `abs()` and
 *   `fabs()` functions by applying both to the integer value -2,
 *   showing that `abs()` returns an integer absolute value while
 *   `fabs()` returns a floating-point absolute value.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define NCALLS 100000000 // number of fct calls
#define NCOLS 8 // number of collumns
#define NLINES 3 // number of lines


//Typedefs

//Prototype Functions

//MAIN
int main(){
    int x = -2;
    printf("abs(x)  = %d\n", abs(x));
    printf("fabs(x) = %f\n", fabs(x));
}

//Helper-Functions