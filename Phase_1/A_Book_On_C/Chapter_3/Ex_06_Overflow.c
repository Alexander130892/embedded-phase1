/*
 * File:    Ex_06_Overflow.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates integer overflow by adding two large
 *   numbers (2000000000 + 2000000000) that exceed the maximum value
 *   for a 32-bit unsigned integer, then prints the result using both
 *   signed and unsigned format specifiers to show the difference in
 *   interpretation.
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
    unsigned int big_big= 2000000000+2000000000;
    printf("%d    %u\n", big_big,big_big);

}

//Helper-Functions