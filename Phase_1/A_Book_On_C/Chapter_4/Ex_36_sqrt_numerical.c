/*
 * File:    Ex_36_sqrt_numerical.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a numerical square root calculation using
 *   Newton's method (also known as the Babylonian method) and tests it
 *   on numbers from 10 to 1000, displaying the computed square root
 *   and number of iterations required for convergence.
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