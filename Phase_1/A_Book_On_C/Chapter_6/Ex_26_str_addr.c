/*
 * File:    Ex_26_str_addr.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates string literal storage behavior in C by
 *   comparing the addresses of two pointers assigned to identical
 *   string literals to show whether the compiler stores them at the
 *   same memory location or creates separate copies.
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