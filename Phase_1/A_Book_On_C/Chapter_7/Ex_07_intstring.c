/*
 * File:    Ex_07_intstring.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This C program demonstrates Binary Coded Decimal (BCD) packing and
 *   unpacking, converting decimal digit strings into compact 32-bit
 *   representations where each digit occupies 4 bits, then recovering
 *   the original string. It includes utility functions for multi-chunk
 *   BCD processing and bit visualization, with the main function
 *   showcasing the pack/unpack cycle on the string "12345678".
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
void stringbitprint(char *s);
uint32_t pack_bcd(const char *s);
void pack_bcd_multi(const char *s, uint32_t *out, int out_len);
void unpack_bcd(uint32_t packed, int num_digits, char *out);

//MAIN
int main(void) {
    const char *original = "12345678";
    char recovered[9];

    uint32_t packed = pack_bcd(original);
    unpack_bcd(packed, 8, recovered);

    printf("original:  %s\n", original);
    printf("packed:    0x%08X\n", packed);
    printf("recovered: %s\n", recovered);

    return EXIT_SUCCESS;
}

void bitprint(int a){
    int i;
    int n = sizeof(int) * CHAR_BIT;
    int mask = 1 << (n-1); // 100..00
    for(i=1; i<=n; i++){
        putchar(((a & mask)==0)?'0':'1');
        a <<= 1;
        if( i % 4 == 0 && i<n)
            putchar(' ');
    }
    putchar('\n');
}

#include <stdint.h>
#include <string.h>

uint32_t pack_bcd(const char *s) {
    uint32_t result = 0;
    int len = strlen(s);

    if (len > 8) len = 8;           /* clamp — only 8 nibbles fit in 32 bits */

    for (int i = 0; i < len; i++) {
        uint32_t nibble = (uint32_t)(s[i] - '0') & 0xF;
        result = (result << 4) | nibble;
    }
    return result;
}

void pack_bcd_multi(const char *s, uint32_t *out, int out_len) {
    int total = strlen(s);
    for (int chunk = 0; chunk < out_len; chunk++) {
        const char *p = s + chunk * 8;
        int remaining = total - chunk * 8;
        if (remaining <= 0) 
        { 
            out[chunk] = 0; 
            continue; 
        }
        out[chunk] = pack_bcd(p);   /* reuses the single-chunk function */
    }
}

void unpack_bcd(uint32_t packed, int num_digits, char *out) {
    for (int i = num_digits - 1; i >= 0; i--) {
        out[i] = (char)((packed & 0xF) + '0');  /* extract lowest nibble */
        packed >>= 4;                             /* shift next nibble into position */
    }
    out[num_digits] = '\0';                       /* null terminate */
}