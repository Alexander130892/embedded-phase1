/*
 * File:    Ex_04_shift_signed.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This file demonstrates the behavior of right bit-shifting
 *   operations on signed versus unsigned integers, specifically
 *   showing whether zeros or ones are shifted in during the operation.
 *   It includes utility functions for cyclic bit shifting and printing
 *   binary representations of values.
 */
//Libraries
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Consts

//Typedefs

//Prototype Functions
short cyclic_shift(unsigned short a, int n);
void bitprint(short);

//MAIN
int main(void){
    int i =-1;
    unsigned int u =-1;
    bitprint(i);
    bitprint(u);
    bitprint(i >> 1);
    bitprint(u >> 1);
    if(i >> 1 == u >> 1)
        printf("Zeros are shifted in .\n");
    else
        printf("Ones are shifted in .\n");
    return EXIT_SUCCESS;
}

short cyclic_shift(unsigned short a, int n){
    
    int size  = sizeof(short) * CHAR_BIT;
    
    n = n % size;                          // handle |n| > 16 too
    if (n < 0) n += size;                  // map negative to equivalent positive
    if (n == 0) return a;
    unsigned short mask = (1u << n) - 1;
    mask <<= size-n;
    unsigned short first_n_bits = a & mask;
    first_n_bits >>= size -n;
    a <<=n;
    return a | first_n_bits;
}

void bitprint(short a){
    int i;
    int n = sizeof(short) * CHAR_BIT;
    int mask = 1 << (n-1); // 100..00
    for(i=1; i<=n; i++){
        putchar(((a & mask)==0)?'0':'1');
        a <<= 1;
        if( i % CHAR_BIT == 0 && i<n)
            putchar(' ');
    }
    putchar('\n');
}