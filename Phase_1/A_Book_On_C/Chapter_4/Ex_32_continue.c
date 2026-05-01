/*
 * File:    Ex_32_continue.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates the behavior of the `continue` statement in
 *   C by executing an infinite loop that prints "124" repeatedly, as
 *   the `continue` statement causes the unreachable code after it
 *   (printing '3' and '5') to be skipped.
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