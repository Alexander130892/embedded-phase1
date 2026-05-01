/*
 * File:    Ex_25_hex.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates hexadecimal number representation in C by
 *   declaring three integer variables with hexadecimal values (0xabc,
 *   0xABc, 0xABC) and printing their decimal equivalents to show that
 *   hexadecimal literals are case-insensitive.
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
    int a = 0xabc;
    int b = 0xABc;
    int c = 0xABC;
    printf("a = %d\nb = %d\nc = %d\n", a,b,c);

}

//Helper-Functions