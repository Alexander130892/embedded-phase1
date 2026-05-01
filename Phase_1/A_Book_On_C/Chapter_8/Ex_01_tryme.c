/*
 * File:    Ex_01_tryme.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program demonstrates macro stringification by defining a
 *   macro PRN(x) that prints the literal text of its argument as a
 *   string, resulting in the output "Hello from main()" when executed.
 */
//write pack into one line
#include <stdio.h>

#define PRN(x) printf(#x "\n");

int main(void){ 
    PRN(Hello from main());
    return 0;
}
