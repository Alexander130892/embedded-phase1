/*
 * File:    Ex_02_sin2cos2.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program demonstrates the fundamental trigonometric identity
 *   sin²(x) + cos²(x) = 1 by calculating and printing the sum of
 *   squared sine and cosine values for x values from 0 to 2π in
 *   increments of 0.1. The output shows the mathematical identity
 *   holds true across the range, with results displayed in hexadecimal
 *   floating-point format for precision verification.
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
    double two_pi = 2.0 * M_PI;
    double h = 0.1;
    double x;
    for (x=0.0; x < two_pi; x+=h){
        printf("%5.1f: %.15a\n", x, sin(x)*sin(x)+cos(x)*cos(x));
    }
}

//Helper-Functions