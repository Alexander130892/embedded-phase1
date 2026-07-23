/*
 * File:    hello.c
 * Author:  Alexander130892
 * Date:    13-6-2026
 *
 * Description:
 *   This program prints a greeting message to standard output and
 *   exits successfully, and is intended to be compiled for the ARM
 *   64-bit (aarch64) architecture.
 */
#include <stdio.h>

int main(void)
{
    printf("Hello from cross-compiled aarch64!\n");
    return 0;
}
