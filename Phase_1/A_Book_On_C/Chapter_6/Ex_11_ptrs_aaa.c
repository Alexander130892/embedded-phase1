/*
 * File:    Ex_11_ptrs_aaa.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates pointer arithmetic by creating an integer
 *   array, setting a pointer to the 4th element (index 3), and
 *   printing the values at that position and the next position using
 *   pointer dereferencing. The program outputs the values 6 and 8 with
 *   placeholder question marks in the format strings.
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

//Prototype


int main() {
    int a[]={0,2,4,6,8},
        *p = a+3;

    printf("%s%d%s\n%s%d%s\n",
        "a[?]   = ", *p,    "?",
        "a[?+1] = ", *(p+1), "?" );
    return 0;
}