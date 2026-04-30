/*
 * File:    Ex_15_char_sizeof.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates the sizeof operator behavior with char
 *   variables and expressions in C, showing how character literals are
 *   treated as integers and how integer promotion affects the size of
 *   char arithmetic operations.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define NCALLS 100000000 // number of fct calls
#define NCOLS 8 // number of collumns
#define NLINES 3 // number of lines


//Typedefs

//Prototype Functions

//MAIN
int main(){
    char c = 'A';
    printf("sizeof(c)       = %lu\n", sizeof(c));
    printf("sizeof('A')     = %lu\n", sizeof('A')); //difference in C and C++
    printf("sizeof(c+c)     = %lu\n", sizeof(c+c)); //integer promotion
    printf("sizeof(c='A')   = %lu\n", sizeof(c='A'));
}

//Helper-Functions