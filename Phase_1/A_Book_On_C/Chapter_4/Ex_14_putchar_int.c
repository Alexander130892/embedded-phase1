/*
 * File:    Ex_14_putchar_int.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates the behavior of putchar() function calls
 *   within a printf statement, showing that putchar() returns an
 *   integer value (the character code) while also printing the
 *   characters to stdout, resulting in the characters being printed
 *   before their corresponding integer values.
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
 
    printf("%d %d %d   \n", putchar('1'), putchar('2'), putchar('3'));
}

//Helper-Functions