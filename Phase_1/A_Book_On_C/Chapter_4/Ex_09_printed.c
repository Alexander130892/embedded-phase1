/*
 * File:    Ex_09_printed.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates C's if-else statement binding behavior by
 *   testing nested conditional statements with variables i=7 and j=7,
 *   showing that the else clause binds to the inner if statement
 *   rather than the outer one, resulting in the output of two values.
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
    int i =7, j=7;
    if (i==1)
        if (j==2)
            printf("%d\n", i=i+j);
        else
            printf("%d\n", i=i-j);
    printf("%d\n", i);
}

//Helper-Functions