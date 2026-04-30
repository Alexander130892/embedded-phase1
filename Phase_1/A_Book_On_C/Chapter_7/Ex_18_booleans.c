/*
 * File:    Ex_18_booleans.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This file demonstrates Boolean logic operations on 4-bit values by
 *   extracting individual bits and displaying truth tables for OR,
 *   AND, and a complex Boolean expression across all possible 16 input
 *   combinations. It serves as an educational example showing how
 *   Boolean operators work with bit manipulation and logical
 *   expressions.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int main(void) {
    unsigned char b;
    int cnt;
    int b1, b2, b3, b4;

    /* extract bits from b — b1 is LSB, b4 is bit 3 */
    #define B1(b) ((b >> 0) & 1)
    #define B2(b) ((b >> 1) & 1)
    #define B3(b) ((b >> 2) & 1)
    #define B4(b) ((b >> 3) & 1)

    cnt = 0;
    printf("\n%5s%5s%5s%5s%5s%5s\n", "Cnt", "b1", "b2", "b3", "b4", "fct1");
    for (b = 0; b < 16; b++)
        printf("%5d%5d%5d%5d%5d%5d\n", cnt++,
               B1(b), B2(b), B3(b), B4(b),
               B1(b) || B2(b) || B3(b) || B4(b));

    cnt = 0;
    printf("\n%5s%5s%5s%5s%5s%5s\n", "Cnt", "b1", "b2", "b3", "b4", "fct2");
    for (b = 0; b < 16; b++)
        printf("%5d%5d%5d%5d%5d%5d\n", cnt++,
               B1(b), B2(b), B3(b), B4(b),
               B1(b) && B2(b) && B3(b) && B4(b));

    cnt = 0;
    printf("\n%5s%5s%5s%5s%5s%5s\n", "Cnt", "b1", "b2", "b3", "b4", "fct3");
    for (b = 0; b < 16; b++)
        printf("%5d%5d%5d%5d%5d%5d\n", cnt++,
               B1(b), B2(b), B3(b), B4(b),
               !(!B1(b) || B2(b)) && (!B3(b) || B4(b)));

    return EXIT_SUCCESS;
}