/*
 * File:    Ex_16_recursion.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates recursion by having the main function call
 *   itself 17 times, printing "The Universe is never ending!" with an
 *   incrementing counter each time, and includes an unused prime
 *   factorization function.
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
    static int cnt=0;
    printf("%d   The Universe is never ending!    ", cnt);
    cnt++;
    if(cnt <17)
        main();
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