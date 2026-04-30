/*
 * File:    Ex_09_printed.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates C's if-else statement association rules by
 *   testing nested conditionals where the variables i and j are both
 *   set to 7, resulting in the else clause executing and printing the
 *   result of i=i-j followed by the final value of i.
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