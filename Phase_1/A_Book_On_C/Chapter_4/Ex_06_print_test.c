/*
 * File:    Ex_06_print_test.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file tests the behavior of the logical NOT operator (!) in C
 *   by printing the results of various expressions involving negation
 *   of characters and integers with different combinations of logical
 *   and arithmetic operators.
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
    char c='A';
    int i=5, j=10;
    printf("%d, %d, %d\n", !c, !!c, !!!c);
    printf("%d, %d, %d\n", -!i, !-c, !-i-!j);
    printf("%d, %d, %d\n", !(6*j+i-c), ! i-5, !(j -10));
}

//Helper-Functions