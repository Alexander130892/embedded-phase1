/*
 * File:    Ex_17_hex_from_string.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates how to parse a hexadecimal string
 *   representation into an integer using sscanf with the %x format
 *   specifier, converting "0xabc" to its decimal equivalent 2748.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    char s[] = "0xabc";
    int n;

    sscanf(s, "%x", &n);
    printf("Value of n: %d\n", n);
    return 0;
}
