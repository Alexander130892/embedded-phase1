/*
 * File:    Ex_08_eps.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program tests floating-point precision by checking if the
 *   expression `1.0 < 1.0 + eps` evaluates to true when eps is set to
 *   1e-15, printing "True" or "False" based on whether the small
 *   epsilon value is significant enough to be detected in
 *   floating-point arithmetic.
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