/*
 * File:    Ex_28_const.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates undefined behavior by attempting to modify
 *   a const char array through a non-const pointer, which violates the
 *   const qualifier and may cause unpredictable results or crashes.
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
    
    const char s[]="char";
    char *p=s;
    *p='A';
    printf("%s\n",s);
    return EXIT_SUCCESS;
}