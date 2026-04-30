/*
 * File:    Ex_18_gcd.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file calculates the greatest common divisor (GCD) of two
 *   integers using the Euclidean algorithm, with both functions
 *   (gcd_rec and gcd_it) actually implemented iteratively despite the
 *   misleading function names. The program demonstrates the GCD
 *   calculation by finding the GCD of 48 and 24, which outputs 24.
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
int gcd_rec(int a, int b);
int gcd_it(int a, int b);

int main(){
    int a=48;
    int b=24;
    printf("gcd(%d,%d)=%d\n", a,b,gcd_rec(a,b));
    
    return 0;
}

int gcd_rec(int a, int b){
    int r;
    while(b){
        r = a %b;
        a = b;
        b = r;
    }
    return a;
}
int gcd_it(int a, int b){
    int r;
        while(b){
    r = a %b;
    a = b;
    b = r;
        }
    return a;
}