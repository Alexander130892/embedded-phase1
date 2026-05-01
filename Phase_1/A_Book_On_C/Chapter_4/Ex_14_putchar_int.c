/*
 * File:    Ex_14_putchar_int.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the behavior of the putchar() function by
 *   showing that it prints characters to stdout immediately but
 *   returns their ASCII values, which are then printed by printf in
 *   reverse order due to function call evaluation order.
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