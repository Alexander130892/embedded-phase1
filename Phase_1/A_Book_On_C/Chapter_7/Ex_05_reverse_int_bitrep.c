/*
 * File:    Ex_05_reverse_int_bitrep.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file prints the binary representation of an integer (0xAAAA)
 *   in both normal bit order and reversed bit order, with spaces
 *   separating each byte. The program demonstrates two different
 *   approaches to displaying bits: left-to-right using left shifts and
 *   a high-order mask, versus right-to-left using right shifts and a
 *   low-order mask.
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
void bitprint(int);
void bitprint_reverse(int a);

//MAIN
int main(void){
    unsigned int i =0xAAAA;
    bitprint(i);
    bitprint_reverse(i);
    return EXIT_SUCCESS;
}

void bitprint(int a){
    int i;
    int n = sizeof(int) * CHAR_BIT;
    int mask = 1 << (n-1); // 100..00
    for(i=1; i<=n; i++){
        putchar(((a & mask)==0)?'0':'1');
        a <<= 1;
        if( i % CHAR_BIT == 0 && i<n)
            putchar(' ');
    }
    putchar('\n');
}

void bitprint_reverse(int a){
    int i;
    int n = sizeof(int) * CHAR_BIT;
    int mask = 1; // 000..01
    for(i=1; i<=n; i++){
        putchar(((a & mask)==0)?'0':'1');
        a >>= 1;
        if( i % CHAR_BIT == 0 && i<n)
            putchar(' ');
    }
    putchar('\n');
}