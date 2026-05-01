/*
 * File:    Ex_21_inf copy.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file computes and prints the mathematical function x^x (x
 *   raised to the power of x) for values of x ranging from 143.0 to
 *   144.9 in increments of 0.1, displaying the results in scientific
 *   notation.
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