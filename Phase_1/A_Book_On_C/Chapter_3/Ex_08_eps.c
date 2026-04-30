/*
 * File:    Ex_08_eps.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file tests floating-point precision by checking whether
 *   adding a very small epsilon value (1e-15) to 1.0 produces a number
 *   that is distinguishable from 1.0 in floating-point arithmetic. It
 *   prints "True" if the comparison detects a difference or "False" if
 *   the epsilon is too small to be represented in the floating-point
 *   format.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Consts
#define NCALLS 100000000 // number of fct calls
#define NCOLS 8 // number of collumns
#define NLINES 3 // number of lines


//Typedefs

//Prototype Functions

//MAIN
int main(){
    double eps=1e-15;
    printf("%s\n", (1.0<1.0+eps)?"True":"False");

}

//Helper-Functions