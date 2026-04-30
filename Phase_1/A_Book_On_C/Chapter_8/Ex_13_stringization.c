/*
 * File:    Ex_13_stringization.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This file demonstrates the C preprocessor's stringization operator
 *   (#) by defining a macro YANK that converts its argument into a
 *   string literal and assigns it to a character pointer. When
 *   executed, it prints the stringized version of the macro argument
 *   "Go home, Yankee!" to the console.
 */
//write pack into one line
#include <stdio.h>

#define YANK(x) s=#x


int main(void){ 
    char*s;
    YANK("Go home, Yankee!");
    printf("%s\n",s);
    return 0;
}
