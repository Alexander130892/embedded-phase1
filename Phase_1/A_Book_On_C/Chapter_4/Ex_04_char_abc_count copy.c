/*
 * File:    Ex_04_char_abc_count copy.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program reads characters from standard input and counts how
 *   many are letters 'a', 'b', or 'c' (case-insensitive) versus all
 *   other characters, then displays the totals. The program contains
 *   unused constants and comments that don't match its actual
 *   functionality.
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
    int c=0, abc=0, othr=0;
    while((c=getchar()) != EOF){
        if((c<='c' && c>='a') || (c<='C' && c>='A') )
            abc++;
        else
            othr++;
    }
    printf("The amount of chars in this file equals: %d, ABC: %d, other %d", abc+othr, abc, othr);

}

//Helper-Functions