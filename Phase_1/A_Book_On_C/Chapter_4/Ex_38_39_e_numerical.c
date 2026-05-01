/*
 * File:    Ex_38_39_e_numerical.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program calculates the mathematical constant e (Euler's
 *   number) using two different numerical methods: the limit
 *   definition (1 + 1/n)^n and the Taylor series expansion (sum of
 *   1/n!). The program iterates each method until convergence and
 *   displays the results along with the number of iterations required.
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
double factorial(int n);
//MAIN
int main(){
   double prev=1;
   double e=2;
   int i=2;
   while((prev-e)){
        prev=e;
        e=pow((1+1.0/i),i);
        i++;
        }
    printf("e = %.6f, in %d iterations\n", e, i);

    double taylor=1;
    prev=0;
    int n=1;
    while(fabs(prev-taylor)>1e-10){
        prev=taylor;
        taylor+=1.0/(factorial(n));
        n++;
        }
     printf("e = %.6f, in %d iterations\n", taylor, n-1);
}
double factorial(int n){
    if(n==0)
        return 1;
    else
        return n*factorial(n-1);
}


//Helper-Functionsd