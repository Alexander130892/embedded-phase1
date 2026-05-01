/*
 * File:    Ex_18_gcd.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements and demonstrates the calculation of the
 *   greatest common divisor (GCD) of two integers using the Euclidean
 *   algorithm, with both recursive and iterative versions defined
 *   (though both are actually implemented iteratively). The program
 *   calculates and displays the GCD of 48 and 24 as an example.
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