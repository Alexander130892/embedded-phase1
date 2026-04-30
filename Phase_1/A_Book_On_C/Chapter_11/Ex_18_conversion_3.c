/*
 * File:    Ex_18_conversion_3.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program demonstrates the behavior of converting a negative
 *   number string ("-1") to an unsigned integer using sscanf, which
 *   results in the value wrapping around to the maximum unsigned
 *   integer value due to how signed-to-unsigned conversion works in C.
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
