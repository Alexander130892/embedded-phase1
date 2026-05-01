/*
 * File:    Ex_05_pow.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates a custom implementation of a power function
 *   by calling pow(2.0, 3.0) and printing the result, but the actual
 *   pow() function implementation is missing from the provided code.
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

