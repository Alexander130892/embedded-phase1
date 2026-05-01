/*
 * File:    Ex_17_overflow.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates integer overflow behavior in C by
 *   performing addition and multiplication operations on an unsigned
 *   integer initialized to the maximum value (UINT_MAX), showing how
 *   the results wrap around when exceeding the data type's limits.
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