/*
 * File:    Ex_44_F_ptr.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates different ways to call functions using
 *   function pointer syntax and excessive parentheses, creating a
 *   chain of function calls from f() to g() to h() that each print a
 *   greeting message.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Consts

//Typedefs

//Prototype Functions
void f(void);
void g(void);
void h(void);

//MAIN
int main(void){
    (*f)();
    return EXIT_SUCCESS;
}

void f(void){
    printf("Hello from f().\n");
    (((*g)))();
}

void g(void){
    printf("Hello from g().\n");
    (*(*(*h)))();
}

void h(void){
    printf("Hello from h().\n");
}