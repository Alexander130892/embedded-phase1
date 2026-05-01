/*
 * File:    Ex_11_isprime.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program implements a prime number checker that tests
 *   whether a given integer is prime by checking for divisors up to
 *   the square root of the number. The main function tests the number
 *   10 and prints whether it is prime or not using the isprime
 *   function.
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
