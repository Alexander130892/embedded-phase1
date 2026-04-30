/*
 * File:    Ex_17_overflow.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates integer overflow behavior in C by
 *   performing addition and multiplication operations on the maximum
 *   unsigned integer value (UINT_MAX) and printing the results to show
 *   how the values wrap around when they exceed the maximum
 *   representable value.
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
    int i;
    unsigned u= UINT_MAX;

    for (i=0;i<10;i++){
        printf("%u + %d = %u\n", u, i, u+i);
    }
    for (i=0;i<10;i++){
        printf("%u * %d = %u\n", u, i, u*i);
    }
}

//Helper-Functions