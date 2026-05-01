/*
 * File:    Ex_23_tryme.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file reads characters from standard input and echoes them to
 *   standard output until EOF is reached, but contains a bug where the
 *   loop variable 'c' is declared as char instead of int, which can
 *   cause issues on platforms where char is unsigned by default since
 *   EOF is typically -1.
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
    char c; // CORRECT version is INT ! platforms where chars are unsigned by default would cause an issue !
    while((c=getchar()) != EOF){
        putchar(c);
        -1;
    }    
}

//Helper-Functions