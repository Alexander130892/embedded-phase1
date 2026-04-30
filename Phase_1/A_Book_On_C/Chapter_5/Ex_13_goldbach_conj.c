/*
 * File:    Ex_13_goldbach_conj.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates Goldbach's conjecture by finding and
 *   printing pairs of prime numbers that sum to each even number in
 *   the range 700-1100. For each even number, it displays the first
 *   valid prime pair decomposition it finds.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <math.h>
#include <limits.h>

//Consts
#define START 700
#define FINISH 1100

//Typedefs

//Prototypes
int isprime(int);

int main(){
    printf("Every even number greater than 2 is the sum of two primes:\n");
    int a,b;
    int n=START;
    while (n <= FINISH){
        for(a=2;a<n;a++){
            b=n-a;
            if(isprime(a) && isprime(b)){
                printf("%d = %d + %d\n", n, a, b);
                n+=2;
                a=n;
            }
        }
    }
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