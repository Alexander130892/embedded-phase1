/*
 * File:    Ex_11_isprime.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program checks if a number is prime by testing divisibility
 *   from 2 up to the square root of the number. The program tests
 *   whether 10 is prime and prints the result, demonstrating the
 *   isprime() function which returns 1 for prime numbers and 0 for
 *   non-prime numbers.
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
