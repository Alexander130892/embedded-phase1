/*
 * File:    Ex_14_greetings.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates C preprocessor macro functionality by
 *   defining a GREETINGS macro that takes three parameters and uses
 *   the stringification operator (#) to convert them into a formatted
 *   greeting message that gets printed to stdout.
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
