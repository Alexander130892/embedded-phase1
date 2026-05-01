/*
 * File:    Ex_25_signed_bitfield.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the behavior of signed bitfields by
 *   continuously incrementing and printing two bitfield members (3-bit
 *   and 4-bit) in an infinite loop, showing how they wrap around due
 *   to their limited bit storage and signed nature.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts


int main(){
    int         i;
    struct test {
    int a:3, b:4;
    } x ={0};

    for ( ; ; )
    {
        printf("x.a = %2d    x.b = %2d    \n", x.a++, x.b++);
    }
    return EXIT_SUCCESS;
}
