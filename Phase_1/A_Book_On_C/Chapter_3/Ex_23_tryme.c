/*
 * File:    Ex_23_tryme.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file reads characters from standard input using getchar() and
 *   echoes them to standard output using putchar() until EOF is
 *   reached, essentially functioning as a simple character echo
 *   program with a meaningless "-1;" statement that has no effect.
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