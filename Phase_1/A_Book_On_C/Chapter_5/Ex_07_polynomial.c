/*
 * File:    Ex_07_polynomial.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program evaluates and prints coordinate pairs of a
 *   quadratic polynomial function f(x) = ax² + bx + c with
 *   coefficients a=1, b=-3, c=2 over the interval x=[0,3] in
 *   increments of 0.1. The polynomial is computed using Horner's
 *   method for efficient evaluation.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <math.h>
#include <limits.h>

//Consts
int z;



//Typedefs

//Prototypes
double f(double,double,double,double);

int main(){
    double a=1,b=-3,c=2;
    for (double x=0;x<=3;x+=0.1){
        printf("%.1lf,%.1lf\n",x,f(a,b,c,x));
    }
    return 0;
}

double f(double a,double b,double c,double x){
    return (a*x+b)*x+c;
}