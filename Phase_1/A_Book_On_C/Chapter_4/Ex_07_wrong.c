/*
 * File:    Ex_07_wrong.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file contains a simple C program that demonstrates a common
 *   programming error where an assignment operator (=) is mistakenly
 *   used instead of a comparison operator (==) in an if statement,
 *   causing the condition to always execute and print "2, 4, 6".
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