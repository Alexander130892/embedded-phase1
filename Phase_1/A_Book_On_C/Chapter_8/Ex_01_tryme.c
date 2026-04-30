/*
 * File:    Ex_01_tryme.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This file demonstrates the use of the C preprocessor's stringizing
 *   operator (#) by defining a macro PRN that converts its argument
 *   into a string literal and prints it, resulting in the output
 *   "Hello from main()".
 */
//write pack into one line
#include <stdio.h>

#define PRN(x) printf(#x "\n");

int main(void){ 
    PRN(Hello from main());
    return 0;
}
