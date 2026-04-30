/*
 * File:    Ex_22_float_promotion.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates that arithmetic operations between float
 *   variables do not get automatically promoted to double precision,
 *   showing the sizeof results for float, double, and a float addition
 *   operation to verify they remain as float type.
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
    float x =1.0, y=2.0;
    printf("%s%lu\n%s%lu\n%s%lu\n",
        "sizeof(float) = ", sizeof(float),
        "sizeof(double) = ", sizeof(double),
        "sizeof(x+y) = ", sizeof(x+y)); // no promotion to double
    
}

//Helper-Functions