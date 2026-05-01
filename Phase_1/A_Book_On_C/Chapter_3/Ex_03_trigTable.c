/*
 * File:    Ex_03_trigTable.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file generates and displays a table of trigonometric values
 *   (sine, cosine, tangent) for angles from 0 to 360 degrees in
 *   increments of 18 degrees. The code incorrectly treats the angle
 *   values as if they are in radians when calling the trigonometric
 *   functions, which expect radian input but the variable naming
 *   suggests degrees.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Consts
#define NCALLS 100000000 // number of fct calls
#define NCOLS 8 // number of collumns
#define NLINES 3 // number of lines


//Typedefs

//Prototype Functions

//MAIN
int main(){
    double two_pi = 360;
    double h = two_pi/20;
    double x;
    printf("%5s: %5s %5s %5s\n", "x", "sin(x)", "cos(x)", "tan(x)");
    for (x=0.0; x < two_pi; x+=h){
        printf("%5.1f: %5.2f %5.2f %5.2f\n", x, sin(x), cos(x), tan(x));
    }
}

//Helper-Functions