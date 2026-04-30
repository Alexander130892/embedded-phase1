/*
 * File:    Ex_08_bitprint_table.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This file prints a table showing the binary representation of
 *   powers of 2 (from 2^0 to 2^31) alongside their corresponding masks
 *   (power minus 1), then demonstrates the bitprint function with a
 *   multi-character constant. The bitprint function displays the
 *   binary representation of integers with spaces separating each byte
 *   for readability.
 */
//Libraries
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Consts
#define N 32
//Typedefs

//Prototype Functions

void bitprint(int);

//MAIN
int main(void){
    for (int i = 0 ; i < N; i++){
        printf("%2d: ",i);
        bitprint(1 << i);
        printf("\t");
        bitprint((1<<i)-1);
        printf("\n");
    }
    bitprint('ab');
    return EXIT_SUCCESS;
}

void bitprint(int a) {
    int i;
    int n = sizeof(int) * CHAR_BIT;
    unsigned int ua = (unsigned int)a;   /* strip signedness from the value */
    unsigned int mask = 1u << (n - 1);     /* 1u ensures unsigned arithmetic */

    for (i = 1; i <= n; i++) {
        putchar(((ua & mask) == 0) ? '0' : '1');
        ua <<= 1;
        if (i % CHAR_BIT == 0 && i < n)
            putchar(' ');
    }
}
