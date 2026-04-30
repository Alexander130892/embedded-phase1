/*
 * File:    Ex_14_prime_factor.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file computes and displays the prime factorization of
 *   integers from 0 to 99, showing each number as a product of its
 *   prime factors separated by " x ". It handles edge cases by
 *   printing "invalid input" for numbers less than 1 and "1" for the
 *   number 1.
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
void prime_factor(int);

int main(){
    for(int i=0;i<NUMBERS;i++){
        printf("%d = ",i);
        prime_factor(i);
        putchar('\n');
    }
    return 0;
}

void prime_factor(int n){
    if (n < 1) {
        printf("invalid input\n");
        return;
    }
    if (n == 1) {
        printf("1");
        return;
    }
    for (int k = 2 ; k <= n;k++ ){
        while(n%k==0){
            printf("%d%s", k, ((n/k)==1)?"":" x ");
            n=n/k;
        }
    }
}