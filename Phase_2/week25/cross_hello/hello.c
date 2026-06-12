/*
 * File:    hello.c
 * Author:  Alexander130892
 * Date:    11-6-2026
 *
 * Description:
 *   This program prints a greeting message indicating successful
 *   cross-compilation for the ARM 64-bit (aarch64) architecture. It
 *   serves as a minimal test to verify that a cross-compilation
 *   toolchain is working correctly.
 */

#include <stdio.h>

int main(void)
{
    printf("Hello from cross-compiled aarch64!\n");
    return 0;
}
