/*
 * File:    Ex_42_f_ptr.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates function pointers in C by passing function
 *   addresses (strcmp and main) to a function that prints their memory
 *   addresses, with special handling to identify the strcmp function
 *   specifically.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Consts

//Typedefs

//Prototype Functions
void tell_me(void *f);

//MAIN
int main(int argc, char **argv){
   tell_me(strcmp);
   tell_me(main);
    return EXIT_SUCCESS;
}

void tell_me(void *f){
    if (f == strcmp)
        printf("Address of strcmp(): %p\n", f);
    else
        printf("Function address: %p\n", f);
}

