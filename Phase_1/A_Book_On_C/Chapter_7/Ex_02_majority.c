/*
 * File:    Ex_02_majority.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This C program implements a bitwise majority function that returns
 *   a value where each bit position is set to 1 if at least two of the
 *   three input values have 1 in that position, and includes a utility
 *   function to print binary representations of the values for
 *   visualization.
 */
//Libraries
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Consts

//Typedefs

//Prototype Functions
short majority(short a, short b, short c);
void bitprint(short);

//MAIN
int main(void){
    short a = 0xAAAA; // 1010 1010
    short b = 0x9A9F; // 1001 1001
    short c = 0x84C8; // 1000 0100
    bitprint(a);
    bitprint(b);
    bitprint(c);   
    printf("%hx\n", majority(a,b,c));
    bitprint(majority(a,b,c));  
    return EXIT_SUCCESS;
}

short majority(short a, short b, short c){
    return ((a & b) | (b & c) | (a & c));
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