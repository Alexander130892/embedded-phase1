/*
 * File:    Ex_12_is_fib_prime.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program calculates Fibonacci numbers for positions 3
 *   through 19 and checks whether both the Fibonacci number and its
 *   position index are prime numbers. It prints formatted output
 *   showing each Fibonacci number, its primality status, and whether
 *   the corresponding position number is also prime.
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
int is_fib_prime(int);
int fib(int);


int main(){
    for(int n=3;n<20;n++){
        printf("the %dth Fibonacci number is %d and is %sprime, while %d is %sprime.\n",n,fib(n), is_fib_prime(n)?"":"NOT ",n, isprime(n)?"":"NOT ");
    }
    return 0;
}


int fib(int n){
    if (n<=1)
        return n;
    else
        return (fib(n-1)+fib(n-2));
}
int is_fib_prime(int n) {
    return (isprime(fib(n)));
}
int isprime(int n) {
    if (n < 2) return 0;
    for (int i = 2; (long long)i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}