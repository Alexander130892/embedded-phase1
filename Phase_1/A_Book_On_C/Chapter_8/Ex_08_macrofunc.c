/*
 * File:    Ex_08_macrofunc.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates C preprocessor functionality by defining a
 *   macro that uses stringification to print three variables with
 *   their names and values, and also shows the difference between
 *   standard ctype functions (tolower, toupper) and their unsafe
 *   variants (_tolower, _toupper).
 */
//write pack into one line
#include <ctype.h>
#include <stdio.h>

#define PRN3(x,y,z) printf(#x " has value %.1f and " #y " has value %.1f and " #z " has value %.1f\n", x,y,z)

int main(void){ 
    printf("%c %c %c %c\n", tolower('a'), _tolower('a'), toupper('a'), _toupper('a'));
    return 0;
}
