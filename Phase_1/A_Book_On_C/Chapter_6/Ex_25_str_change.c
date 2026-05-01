/*
 * File:    Ex_25_str_change.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the difference between modifying string
 *   arrays versus string literals in C, specifically showing that
 *   attempting to modify a string literal through a pointer results in
 *   undefined behavior (likely a segmentation fault).
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
    //char p[] = "abc"; // works (array)
    char *p = "abc"; // does nto work, pointer to string literal
    *p = 'X';
    printf("%s\n",p);
    return EXIT_SUCCESS;
}