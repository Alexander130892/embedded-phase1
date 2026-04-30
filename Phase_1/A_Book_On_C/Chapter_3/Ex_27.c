/*
 * File:    Ex_27.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file tests whether an unsigned char variable initialized to
 *   0xff (255 in decimal) correctly equals 0xff, printing "Truth!" if
 *   the comparison succeeds or "Explain!" if it fails.
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
    unsigned char c = 0xff;
    if (c == 0xff)
        printf("Truth!");
    else
        printf("Explain!");

}

//Helper-Functions