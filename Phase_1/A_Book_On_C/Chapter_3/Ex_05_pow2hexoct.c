/*
 * File:    Ex_05_pow2hexoct.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program prints the first 34 powers of 2 (from 2^0 to 2^33) in
 *   five different number formats: signed decimal, unsigned decimal,
 *   hexadecimal, octal, and hexadecimal floating-point notation. Each
 *   iteration doubles the value and displays it across multiple
 *   columns with 15-character field widths.
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
    int i, val=1;
    for (i=1;i<35;i++){
        printf("%15d%15u%15x%15o%15a\n", val,val,val,val,val);
        val *=2;
    }

}

//Helper-Functions