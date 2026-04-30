/*
 * File:    Ex_14_greetings.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This file demonstrates C preprocessor string manipulation by
 *   defining a macro that converts three parameter names into a
 *   formatted greeting string using the stringizing operator (#),
 *   printing "Alice, Bob, and Carole: Hello!" when executed.
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
