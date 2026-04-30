/*
 * File:    Ex_33_min_xyz.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file finds and prints the maximum of three integer variables
 *   (x=1, y=2, z=3) using nested ternary operators, followed by
 *   syntactically incorrect switch statements that would cause
 *   compilation errors.
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