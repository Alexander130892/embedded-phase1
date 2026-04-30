/*
 * File:    Ex_14_greetings.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program defines a macro that takes three arguments and prints
 *   a greeting message to each of them, demonstrating the use of the
 *   stringification operator (#) to convert macro parameters into
 *   string literals.
 */
//write pack into one line
#include <stdio.h>
#undef TRY_ME

#define GREETINGS(a,b,c)\
        printf(#a ", " #b ", and " #c ": Hello!\n")


int main(void){ 
    GREETINGS(Alice, Bob, Carole);
    return 0;
}
