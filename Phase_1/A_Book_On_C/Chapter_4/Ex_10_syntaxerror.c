/*
 * File:    Ex_10_syntaxerror.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates a simple do-while loop that calculates and
 *   prints values using the formula j = 2*i + 3 for i from 0 to 9. The
 *   program contains unused constants and includes that suggest it was
 *   part of a larger exercise or template.
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
    int i=0,j=0;
    do{
        j=2*i+3;
        printf("%d\n", j);
    }while(++i < 10);  
    

}

//Helper-Functions