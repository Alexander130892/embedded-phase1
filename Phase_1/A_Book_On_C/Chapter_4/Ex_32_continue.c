/*
 * File:    Ex_32_continue.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the behavior of the `continue` statement in
 *   C by printing characters in a loop where the continue statement
 *   causes an infinite loop that skips the unreachable code after it.
 *   The program outputs "1" followed by an endless sequence of "24"
 *   because the continue statement prevents execution from reaching
 *   the lines that would print "3" and "5".
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