/*
 * File:    Ex_01_tryme.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the stringification operator (#) in C
 *   macros by defining a PRN macro that converts its argument into a
 *   string literal and prints it, resulting in the output "Hello from
 *   main()".
 */
//write pack into one line
#include <stdio.h>

#define PRN(x) printf(#x "\n");

int main(void){ 
    PRN(Hello from main());
    return 0;
}
