/*
 * File:    Ex_32_continue.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates the behavior of the `continue` statement in
 *   C by printing characters in a specific pattern, where the infinite
 *   while loop causes '1', '2', and '4' to be printed repeatedly while
 *   '3' and '5' are never executed due to the continue statement
 *   skipping the rest of the loop body.
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
    putchar('1');    while(putchar('2')){
        putchar('4');
        continue;
        putchar('3');
        putchar('5');
    }
}

//Helper-Functionsd