/*
 * File:    Ex_01_pow_xn copy 2.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program implements a custom power function that calculates
 *   x raised to the power of n using iterative multiplication, and
 *   demonstrates it by computing 3.5^7.
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
double power(double x, int n);
//MAIN
int main(){
    printf("%.2f\n", power(3.5,7));
}
double power(double x, int n){
    double p=1;
    while(n>0){
        p*=x;
        n--;
    }
        

    return p;
}
