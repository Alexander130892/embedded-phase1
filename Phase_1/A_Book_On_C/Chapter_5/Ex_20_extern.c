/*
 * File:    Ex_20_extern.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the use of the `extern` keyword in C by
 *   declaring external variables `a`, `b`, and `c` in main() and then
 *   defining them as static variables at the end of the file, printing
 *   their values (1, 2, 3) to show how external variable declarations
 *   work within the same source file.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define NUMBERS 100

//Typedefs

//Prototypes

int main(){
    extern int a,b,c;
    printf("%3d%3d%3d\n",a,b,c);
    
    return 0;
}

static int a=1, b=2, c=3;
