/*
 * File:    Ex_08_ptrs_addr_types.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates pointer arithmetic by showing how adding
 *   integers to pointers of different data types (char, int, double,
 *   long double) results in different memory address increments based
 *   on the size of each data type. It prints the memory addresses
 *   after incrementing each pointer type by 1 and 3 to illustrate how
 *   pointer arithmetic scales by the size of the pointed-to type
 *   rather than by single bytes.
 */
 //Libraries
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define COUNT 1000000.0

//Typedefs

//Prototypes


int main() {
    char *pc =NULL;
    int *pi =NULL;
    double *pd =NULL;
    long double *pld =NULL;

    printf("%5lu%5lu\n%5lu%5lu\n%5lu%5lu\n",
        (unsigned long)(pc+1),(unsigned long)(pi+1),
        (unsigned long)(pd+1),(unsigned long)(pld+1),
        (unsigned long)(pc+3),(unsigned long)(pld+3));
    return 0;
}