/*
 * File:    Ex_02_sin2cos2.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file calculates and prints the value of sin²(x) + cos²(x) for
 *   x values from 0 to 2π in increments of 0.1, demonstrating the
 *   fundamental trigonometric identity that this expression always
 *   equals 1.
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