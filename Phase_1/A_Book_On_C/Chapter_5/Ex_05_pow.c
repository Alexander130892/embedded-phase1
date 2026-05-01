/*
 * File:    Ex_05_pow.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates a custom implementation of a power function
 *   by calling pow(2.0, 3.0) to calculate 2 raised to the power of 3,
 *   but the actual pow() function implementation is missing from the
 *   provided code.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <math.h>
#include <limits.h>

//Consts
int z;



//Typedefs

//Prototypes
double pow();

int main(){
    printf("pow(2, 3) = %g\n", pow(2.0, 3.0));
    return 0;
}

