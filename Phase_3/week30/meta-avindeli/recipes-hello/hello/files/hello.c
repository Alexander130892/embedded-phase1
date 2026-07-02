/*
 * File:    hello.c
 * Author:  Alexander130892
 * Date:    2-7-2026
 *
 * Description:
 *   This program prints a greeting message to standard output and
 *   exits successfully. It is a simple test program designed to verify
 *   that cross-compilation for the ARM 64-bit (aarch64) architecture
 *   is working correctly.
 */
#include <stdio.h>

int main(void)
{
    printf("Hello from cross-compiled aarch64!\n");
    return 0;
}
