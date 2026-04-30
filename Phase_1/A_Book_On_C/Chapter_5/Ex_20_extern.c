/*
 * File:    Ex_20_extern.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates the extern keyword in C by declaring
 *   external variables a, b, and c in main() and then defining them as
 *   static variables with values 1, 2, and 3, which are printed to
 *   show how extern can reference variables defined later in the same
 *   file.
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
