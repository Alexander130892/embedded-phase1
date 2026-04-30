/*
 * File:    Ex_39_my_echo.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a simple echo program that prints all
 *   command-line arguments (including the program name) separated by
 *   spaces, followed by a newline.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Consts

//Typedefs

//Prototype Functions
void print_arg(const char *s, int cap);

//MAIN
int main(int argc, char **argv){
    while(*argv != NULL)
        printf("%s ", *argv++);
    putchar('\n');
    return EXIT_SUCCESS;
}

