/*
 * File:    Ex_20_SC_Evaluation.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C file demonstrates short-circuit evaluation behavior of
 *   logical operators (&& and ||) by showing how the right-hand side
 *   expressions are either evaluated or skipped based on the left-hand
 *   side values. It prints the values of variables to illustrate when
 *   assignments occur or are bypassed due to short-circuiting.
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
    int a=0,b=0,x;
    x=0 && (a=b=777);
    printf("%d %d %d   \n", a,b,x);
    x=777 || (a=++b);
    printf("%d %d %d   \n", a,b,x);
}

//Helper-Functions