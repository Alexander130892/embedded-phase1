/*
 * File:    Ex_12_char.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates character overflow behavior by assigning
 *   values 256 and 257 to char variables and printing their actual
 *   stored values, showing how chars wrap around due to their limited
 *   range.
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
    char c1=256,c2=257;
    printf("c1 = %d\nc2 = %d\n", c1,c2);

}

//Helper-Functions