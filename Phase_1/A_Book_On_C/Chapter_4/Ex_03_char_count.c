/*
 * File:    Ex_03_char_count.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program reads characters from standard input until EOF and
 *   counts how many are digits (0-9) versus other characters, then
 *   prints the total character count along with the breakdown of
 *   digits and non-digits.
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
    int c=0, dgt=0, othr=0;
    while((c=getchar()) != EOF){
        if(c<='9' && c>='0')
            dgt++;
        else
            othr++;
    }
    printf("The amount of chars in this file equals: %d, digits: %d, other %d", dgt+othr, dgt, othr);

}

//Helper-Functions