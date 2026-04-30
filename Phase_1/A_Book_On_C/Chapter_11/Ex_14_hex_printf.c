/*
 * File:    Ex_14_hex_printf.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates hexadecimal formatting in C by printing the
 *   decimal number 11259375 in both lowercase and uppercase
 *   hexadecimal format using the %#x and %#X printf format specifiers,
 *   which include the "0x" prefix.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int main(void)
{   
    printf("11259375 = %#x\n", 11259375);
    printf("11259375 = %#X\n", 11259375);
    return 0;
}

