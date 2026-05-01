/*
 * File:    Ex_17_factorial.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file calculates and prints the factorial of 5 using a
 *   recursive function implementation. The program outputs the result
 *   (120) along with the message "The Universe is never ending!" and
 *   includes an unused static counter variable.
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
int factorial(int n);

int main(){
    static int cnt=0;
    printf("%d   The Universe is never ending!    ", factorial(5));
    
    return 0;
}

int factorial(int n){
    if(n==0 || n==1)
        return 1;
    else
        return n*factorial(--n);
}