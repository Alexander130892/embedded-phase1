/*
 * File:    Ex_16_conversion_characters2.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the difference between printf format
 *   specifiers "%0+17d" and "%+017d" when formatting positive
 *   integers, showing how the order of flags (zero-padding and plus
 *   sign) affects the output formatting.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    printf("%0+17d\n", 1);
    printf("%+017d\n", 1);
    return 0;
}
