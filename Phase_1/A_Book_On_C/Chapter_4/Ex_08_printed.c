/*
 * File:    Ex_08_printed.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program demonstrates boolean expression evaluation and
 *   operator precedence by printing the results of various logical
 *   comparisons involving character equality, logical OR operations,
 *   negation operators, and floating-point arithmetic comparisons.
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