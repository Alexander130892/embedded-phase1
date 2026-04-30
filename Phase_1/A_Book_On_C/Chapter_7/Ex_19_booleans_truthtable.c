/*
 * File:    Ex_19_booleans_truthtable.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This file generates and displays a truth table for three different
 *   boolean functions (OR, AND, and a complex expression) applied to
 *   4-bit inputs, using precomputed lookup tables stored as uint16_t
 *   values where each bit position represents the function result for
 *   that input combination.
 */
#include <stdio.h>
#include <stdint.h>

int main(void) {
    unsigned char b;
    int cnt;

    /*
     * Each uint16_t is a lookup table — bit b = result for input b.
     * Computed once, used for all 16 inputs in a single operation.
     *
     * fct1: b1||b2||b3||b4 — true for all b except 0
     * fct2: b1&&b2&&b3&&b4 — true only for b=15 (0b1111)
     * fct3: !(!b1||b2)&&(!b3||b4) — true for b=1,9,13
     */
    uint16_t fct1 = 0xFFFE;   /* 1111111111111110 */
    uint16_t fct2 = 0x8000;   /* 1000000000000000 */
    uint16_t fct3 = 0x2202;   /* 0010001000000010 */

    #define B1(b) ((b >> 0) & 1)
    #define B2(b) ((b >> 1) & 1)
    #define B3(b) ((b >> 2) & 1)
    #define B4(b) ((b >> 3) & 1)
    #define LOOKUP(table, b) ((table >> b) & 1)

    cnt = 0;
    printf("\n%5s%5s%5s%5s%5s%5s%5s%5s\n",
           "Cnt","b1","b2","b3","b4","fct1","fct2","fct3");
    for (b = 0; b < 16; b++)
        printf("%5d%5d%5d%5d%5d%5d%5d%5d\n", cnt++,
               B1(b), B2(b), B3(b), B4(b),
               LOOKUP(fct1, b),
               LOOKUP(fct2, b),
               LOOKUP(fct3, b));

    return 0;
}
