/*
 * File:    Ex_24_condensed_pack.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program packs four individual char values into a single
 *   unsigned int by shifting and combining their bits, then prints the
 *   binary representation of the packed result with spaces separating
 *   each byte.
 */
//write pack into one line
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

unsigned int pack(char a, char b, char c, char d);
void bitprint(unsigned int a);

int main(void){ 
    char a=0x11;
    char b=0xaf;
    char c=0x55;
    char d=0x01;
    bitprint(pack(a,b,c,d));

    return EXIT_SUCCESS;
}

unsigned int pack(char a, char b, char c, char d){
    return ((((((((unsigned char) a) << CHAR_BIT) | (unsigned char) b) << CHAR_BIT) | (unsigned char) c) << CHAR_BIT) | (unsigned char) d);
}

void bitprint(unsigned int a){
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