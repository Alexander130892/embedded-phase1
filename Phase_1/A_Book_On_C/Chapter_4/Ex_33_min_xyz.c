/*
 * File:    Ex_33_min_xyz.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program finds and prints the maximum value among three
 *   integer variables (x=1, y=2, z=3) using nested ternary operators,
 *   followed by an incomplete switch statement that appears to be a
 *   syntax error.
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
    int x=1,y=2,z=3;
    printf("%d\n",(((x>y)?x:y)>z)?((x>y)?x:y):z);
    switch(1)   switch(1);
}

//Helper-Functionsd