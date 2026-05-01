/*
 * File:    Ex_09_printed.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates C's if-else statement precedence and
 *   associativity by testing nested conditional logic where an outer
 *   if condition (i==1) is false, causing the entire nested if-else
 *   block to be skipped and leaving the variable i unchanged. The
 *   program prints the final value of i (which remains 7) to show that
 *   no arithmetic operations were performed due to the failed initial
 *   condition.
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