/*
 * File:    Ex_08_printed.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates C programming concepts by printing the
 *   results of various logical and comparison expressions, including
 *   character comparisons, logical operators, and floating-point
 *   arithmetic comparisons. It serves as an educational example
 *   showing how C evaluates different types of boolean expressions and
 *   outputs their numeric results (0 for false, 1 for true).
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
    double x = 1e+33, y=0.001;
    printf("%d, %d, %d\n", c=='a', c=='a', c!='c');
    printf("%d\n",'B' || 'C');
    printf("%d\n", 1 != !!c == !!!c);
    printf("%d\n", x+y >x-y);

}

//Helper-Functions