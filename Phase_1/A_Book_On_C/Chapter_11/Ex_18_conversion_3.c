/*
 * File:    Ex_18_conversion_3.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates the behavior of converting a negative
 *   number string to an unsigned integer using sscanf, which results
 *   in the negative value being interpreted as a large positive
 *   unsigned value due to two's complement representation.
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    char s[] = "-1";
    unsigned n;

    sscanf(s, "%u", &n);
    printf("Value of n: %u\n", n);
    return 0;
}
