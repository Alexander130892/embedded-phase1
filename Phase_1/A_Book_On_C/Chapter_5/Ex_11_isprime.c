/*
 * File:    Ex_11_isprime.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a prime number checking function that tests
 *   whether a given integer is prime by checking for divisors up to
 *   its square root. The main function demonstrates the isprime
 *   function by testing whether the number 10 is prime and prints the
 *   result.
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
int isprime(int);

int main(){
    int n=10;
    printf("%d is %sprime\n",n,isprime(n)?"":"NOT ");
    return 0;
}

int isprime(int n) {
    if (n < 2) return 0;
    for (int i = 2; (long long)i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}
