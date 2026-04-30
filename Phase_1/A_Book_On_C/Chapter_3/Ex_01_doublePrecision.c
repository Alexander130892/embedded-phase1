/*
 * File:    Ex_01_doublePrecision.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates the precision limitations of
 *   double-precision floating-point numbers by printing the same value
 *   with different levels of precision using both decimal and
 *   hexadecimal formats. It shows how trailing digits beyond double
 *   precision are lost or approximated when stored in memory.
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
    double x = 123.12345678901234567;
    double y = 123.12345678901234000;
    printf("%.17f\n%.17f\n", x,y);
    printf("%a\n%a\n", x,y);
}

//Helper-Functions