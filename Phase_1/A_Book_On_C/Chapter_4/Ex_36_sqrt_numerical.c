/*
 * File:    Ex_36_sqrt_numerical.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file implements Newton's method (Babylonian method) to
 *   numerically calculate square roots of integers from 10 to 1000,
 *   displaying each result with the number of iterations required for
 *   convergence. The program uses an iterative formula that refines
 *   the square root approximation until the previous and current
 *   values are equal.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define ACC 100 // number of fct calls


//Typedefs

//Prototype Functions

//MAIN
int main(){
   double prev;
   double sqrt=1;
   int i=0;
   int n=100;
   double a=25000;
   //scanf("%lf",&a);
   for (int j=10;j<=10*n;j++){
        sqrt=1,prev=0,i=0;
        while((prev-sqrt)){
                prev=sqrt;
                sqrt=0.5*(prev+j/prev);
                i++;
        }
        printf("sqrt(%d) = %.2f, in %d iterations\n", j, sqrt, i);
   }
}

//Helper-Functionsd