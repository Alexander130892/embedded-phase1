/*
 * File:    Ex_29_mistery_program.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This program uses a union and endian-specific hexadecimal
 *   constants to encode and decode the string "Hello, world!" by
 *   storing it as 32-bit integers and then interpreting the memory as
 *   characters. The program demonstrates how byte order affects the
 *   representation of multi-byte data in memory by printing the
 *   decoded message.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define HEX0    0x6c6c6548
    #define HEX1    0x77202c6f
    #define HEX2    0x646c726f
    #define HEX3    0x000000a21
#else
    #define HEX0    0x48656c6c
    #define HEX1    0x6f2c2077
    #define HEX2    0x6f726c64
    #define HEX3    0x210a0000
#endif

//Consts
typedef union{
    char what[16];
    uint32_t cipher[4];
}mystery;

int main(){
    mystery x;

    x.cipher[0] = HEX0;
    x.cipher[1] = HEX1;
    x.cipher[2] = HEX2;
    x.cipher[3] = HEX3;
    printf("%s\n", x.what);
    return EXIT_SUCCESS;
}
