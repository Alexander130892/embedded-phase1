/*
 * File:    Ex_01_equivalent_expr.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates the logical equivalence between two
 *   conditional expressions by showing that `!(a>b)` and `a<b` produce
 *   the same result when a=1 and b=2, printing "Truth!" twice to
 *   verify both conditions evaluate to true.
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
    int a=1,b=2;
    
    if (!(a>b))
        printf("Truth!\n");
    if (a<b)
        printf("Truth!\n");

}

//Helper-Functions