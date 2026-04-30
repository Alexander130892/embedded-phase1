/*
 * File:    Ex_06_extract_even_bits.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This file implements two functions that extract bits at even
 *   positions (0, 2, 4, ..., 30) from a 32-bit input and pack them
 *   consecutively into a 16-bit output. The first function uses an
 *   efficient bit manipulation approach with masks and shifts, while
 *   the second uses a straightforward loop-based method.
 */
#include <stdint.h>

/**
 * Extracts bits at positions 0, 2, 4, ..., 30.
 * Result: bit 0 of input becomes bit 0 of output, 
 * bit 2 becomes bit 1, bit 4 becomes bit 2, etc.
 */
uint16_t extract_even_bits(uint32_t x) {
    // 1. Mask out odd bits: x = x & 01010101...
    // Mask: 0x55555555
    x = x & 0x55555555;

    // 2. Shift and combine to close the gaps
    // From: .a.b.c.d.e.f.g.h (where . is a gap)
    x = (x | (x >> 1)) & 0x33333333; // Groups of 2
    x = (x | (x >> 2)) & 0x0F0F0F0F; // Groups of 4
    x = (x | (x >> 4)) & 0x00FF00FF; // Groups of 8
    x = (x | (x >> 8)) & 0x0000FFFF; // Final 16 bits

    return (uint16_t)x;
}


uint16_t extract_every_other_bit(uint32_t input) {
    uint16_t result = 0;
    
    for (int i = 0; i < 16; i++) {
        // 1. Isolate the bit at position (i * 2)
        // 2. Shift it right so it sits at bit 0
        // 3. Shift it left to its new 'packed' position (i)
        // 4. OR it into the result
        uint32_t bit = (input >> (i * 2)) & 1;
        result |= (uint16_t)(bit << i);
    }
    
    return result;
}