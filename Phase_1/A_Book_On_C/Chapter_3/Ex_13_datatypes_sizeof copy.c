/*
 * File:    Ex_13_datatypes_sizeof copy.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates the memory sizes of various C data types by
 *   using the sizeof operator to print the byte sizes of char, short,
 *   int, unsigned, long, float, double, and long double types.
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
    printf("char = %lu\nshort = %lu\nint = %lu\nunsigned = %lu\nlong = %lu\nfloat = %lu\ndouble = %lu\nlong double = %lu\n", 
        sizeof(char),sizeof(short),sizeof(int),sizeof(unsigned),sizeof(long),sizeof(float),sizeof(double),sizeof(long double));

}

//Helper-Functions