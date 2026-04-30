/*
 * File:    Ex_20_SC_Evaluation.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates short-circuit evaluation in C by showing
 *   how logical operators (&& and ||) can prevent the execution of
 *   subsequent expressions when the result is already determined, as
 *   evidenced by variables remaining unchanged when their assignment
 *   expressions are short-circuited.
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