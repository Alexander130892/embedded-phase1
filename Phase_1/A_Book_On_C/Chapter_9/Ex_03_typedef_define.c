/*
 * File:    Ex_03_typedef_define.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This file demonstrates the difference between using `#define` and
 *   `typedef` to create a string alias for `char*`, showing how
 *   `#define` can cause unexpected behavior when declaring arrays of
 *   strings. The program prints a sentence by concatenating string
 *   arrays, but the `#define` approach would cause array `b` to be
 *   declared as `char` instead of `char*` if uncommented.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define ACC 100 // number of fct calls
#define string char*

//Typedefs
//typedef char* string;



int main(){
    string  a[] = {"I", "like", "to", "fight,"}, // with define, b is a char iso char*
            b[] = {"pinch", "and", "bite."};
    printf("%s %s %s %s %s %s %s\n", a[0],a[1],a[2],a[3],b[0],b[1],b[2] );
    
    return EXIT_SUCCESS;
}
