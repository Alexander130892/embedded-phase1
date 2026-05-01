/*
 * File:    Ex_15_conversion_characters.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the `%n` conversion specifier in printf,
 *   which writes the number of characters printed so far to integer
 *   pointer arguments, then displays the stored values to show how
 *   many characters were output at each `%n` position.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    int a, b, c;
    printf("a%nb%nc%n", &a, &b, &c);   // %n writes happen here
    printf(" %d %d %d\n", a, b, c);    // now a,b,c are initialized
    return 0;
}
