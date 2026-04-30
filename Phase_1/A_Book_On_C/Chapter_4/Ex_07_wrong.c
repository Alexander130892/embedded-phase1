/*
 * File:    Ex_07_wrong.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program demonstrates a common programming error by using
 *   assignment (=) instead of comparison (==) in an if statement,
 *   which will always execute the printf statement and output "2, 4,
 *   6" regardless of the intended logic.
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
    
    int i;
    if(i=2){
        printf("%d, %d, %d\n", i, i+2, i+4);

    }
}

//Helper-Functions