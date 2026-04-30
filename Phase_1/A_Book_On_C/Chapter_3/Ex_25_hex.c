/*
 * File:    Ex_25_hex.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program demonstrates that hexadecimal literals are
 *   case-insensitive by declaring three integer variables with the
 *   same hexadecimal value (0xABC) written in different cases and
 *   printing their decimal equivalents to show they are all equal to
 *   2748.
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