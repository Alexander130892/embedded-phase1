/*
 * File:    hardware_register_struct.c
 * Author:  Alexander130892
 * Date:    3-4-2026
 *
 * Description:
 *   This file demonstrates how to use a C union with bit fields to
 *   model a hardware register, allowing access to the same 16-bit data
 *   either as a raw value or as individual bit fields (ACK, INT flags
 *   and an 8-bit MEM field). It tests various operations including
 *   setting individual bits, boundary conditions, and overflow
 *   behavior to verify the bit field implementation works correctly.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//Consts

//Macros

//Typedefs
typedef union{
    uint16_t raw;
    struct{
        unsigned char ACK: 1;
        unsigned char INT: 1;
        unsigned char :0; //padding
        unsigned char MEM: 8;
    } bits;
}reg_t;


//Prototype Functions


//MAIN 
int main(){
    reg_t r = {0};

    // Test 1 — write via raw, read via bits
    printf("=== Test 1: raw write, bits read ===\n");
    r.raw = 0b1100000011;  // ACK=1, INT=1, MEM=0b00000011 (3)
    printf("raw     : 0x%04X\n", r.raw);
    printf("ACK     : %d (expect 1)\n", r.bits.ACK);
    printf("INT     : %d (expect 1)\n", r.bits.INT);
    printf("MEM     : %d (expect 3)\n", r.bits.MEM);

    // Test 2 — write via bits, read via raw
    printf("\n=== Test 2: bits write, raw read ===\n");
    r.raw       = 0;
    r.bits.ACK  = 1;
    r.bits.INT  = 0;
    r.bits.MEM  = 0xFF;
    printf("raw     : 0x%04X (expect 0x3FC1)\n", r.raw);

    // Test 3 — clear and set individual flags
    printf("\n=== Test 3: flag manipulation ===\n");
    r.bits.ACK = 0;
    printf("ACK cleared: %d (expect 0)\n", r.bits.ACK);
    r.bits.ACK = 1;
    printf("ACK set    : %d (expect 1)\n", r.bits.ACK);

    // Test 4 — MEM boundary values
    printf("\n=== Test 4: MEM boundaries ===\n");
    r.bits.MEM = 0;
    printf("MEM min : %d (expect 0)\n",   r.bits.MEM);
    r.bits.MEM = 255;
    printf("MEM max : %d (expect 255)\n", r.bits.MEM);
    r.bits.MEM = 256;  // overflows 8 bits — what happens?
    printf("MEM overflow: %d (expect 0)\n", r.bits.MEM);

    // Test 5 — zero out everything
    printf("\n=== Test 5: full reset ===\n");
    r.raw = 0;
    printf("raw     : 0x%04X (expect 0x0000)\n", r.raw);
    printf("ACK     : %d (expect 0)\n", r.bits.ACK);
    printf("INT     : %d (expect 0)\n", r.bits.INT);
    printf("MEM     : %d (expect 0)\n", r.bits.MEM);

    return EXIT_SUCCESS;
}