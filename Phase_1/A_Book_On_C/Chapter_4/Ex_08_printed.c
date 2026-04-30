/*
 * File:    Ex_08_printed.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program demonstrates various boolean expressions and
 *   logical operations by printing the results of character
 *   comparisons, logical OR operations, negation operations, and
 *   floating-point arithmetic comparisons. It serves as an educational
 *   example showing how different logical and comparison operators
 *   behave in C, particularly with edge cases involving very large
 *   floating-point numbers.
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