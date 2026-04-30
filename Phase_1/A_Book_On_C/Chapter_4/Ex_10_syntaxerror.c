/*
 * File:    Ex_10_syntaxerror.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates a simple do-while loop that calculates and
 *   prints values using the formula j=2*i+3 for i from 0 to 9,
 *   producing output from 3 to 21. Despite its filename suggesting a
 *   syntax error, the code compiles and runs correctly, though it
 *   contains unused constants and headers.
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