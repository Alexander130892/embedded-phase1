/*
 * File:    Ex_25_signed_bitfield.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This file demonstrates the overflow behavior of signed bitfields
 *   by continuously incrementing and printing two bitfield members (a
 *   3-bit and 4-bit field) in an infinite loop. It shows how signed
 *   bitfields wrap around when they exceed their maximum representable
 *   values, with the 3-bit field cycling through values -4 to 3 and
 *   the 4-bit field cycling through values -8 to 7.
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
