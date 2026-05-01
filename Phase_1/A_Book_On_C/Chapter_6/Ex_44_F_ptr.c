/*
 * File:    Ex_44_F_ptr.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program demonstrates different syntaxes for calling
 *   functions through function pointers, with each function (f, g, h)
 *   using increasingly complex pointer dereference notation to call
 *   the next function in sequence.
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