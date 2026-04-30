/*
 * File:    Ex_03_char_count.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program reads character input from stdin until EOF and counts
 *   the total number of characters, separating them into digits (0-9)
 *   and non-digit characters, then displays the counts.
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