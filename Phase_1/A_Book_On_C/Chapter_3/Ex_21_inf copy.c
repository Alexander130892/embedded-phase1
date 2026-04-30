/*
 * File:    Ex_21_inf copy.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file computes and displays the power function x^x for values
 *   of x ranging from 143.0 to 144.9 in increments of 0.1, printing
 *   the results in scientific notation with 7 decimal places.
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
    
    for(double x=143;x<145;x+=0.1)
        printf("pow(%.1f, %.1f) = %.7e\n", x, x, pow(x,x));
    
}

//Helper-Functions