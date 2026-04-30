/*
 * File:    Ex_05_ptradrr.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates memory addresses of local variables by
 *   declaring three char variables and three char pointers, then
 *   printing their memory addresses as unsigned long integers to show
 *   how variables are arranged in memory.
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
    char a,b,c,*p,*q,*r;
    printf("%lu %lu %lu %lu %lu %lu\n",(unsigned long)&a,(unsigned long)&b,(unsigned long)&c,(unsigned long)&p,(unsigned long)&q,(unsigned long)&r);
    return 0;
}