/*
 * File:    Ex_14_hex_printf.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates hexadecimal formatting in printf by
 *   converting the decimal number 11259375 to hexadecimal
 *   representation, showing both lowercase (%#x) and uppercase (%#X)
 *   formats with the "0x" prefix.
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

