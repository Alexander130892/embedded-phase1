/*
 * File:    Ex_26_str_addr.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates whether two string literals with identical
 *   content ("abc") are stored at the same memory address by comparing
 *   their pointer values and printing the result.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//Consts
#define N 40

//Typedefs
typedef const char cchar;

//Prototype Functions


//MAIN
int main(){
    char *q = "abc"; 
    char *p = "abc";
    if (p ==q)
        printf("Same address!\n");
    else
        printf("Different address!\n");
    return EXIT_SUCCESS;
}