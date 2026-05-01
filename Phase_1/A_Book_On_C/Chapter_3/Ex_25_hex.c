/*
 * File:    Ex_25_hex.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates hexadecimal number literals in C by
 *   declaring three integer variables with hex values (0xabc, 0xABc,
 *   0xABC) and printing their decimal equivalents to show that
 *   hexadecimal notation is case-insensitive.
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