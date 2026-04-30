/*
 * File:    Ex_03_cyclic_shift.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This file implements a cyclic left shift operation on unsigned
 *   short integers, where bits that are shifted out from the left are
 *   rotated back in from the right, and includes a utility function to
 *   print the binary representation of the values for demonstration
 *   purposes.
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
    short b = 0x9A9F; // 1001 1001
    short c = 0x84C8; // 1000 0100
    bitprint(b);
    bitprint(c);   
    //printf("%hx\n", cyclic_shift(b,3));
    bitprint(cyclic_shift(c,-3));  
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