/*
 * File:    Ex_08_macrofunc.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This file demonstrates C macro functionality by defining a macro
 *   PRN3 that uses string concatenation to print three variables with
 *   their names and values, though the main function only calls
 *   character case conversion functions and doesn't actually use the
 *   defined macro.
 */
//write pack into one line
#include <ctype.h>
#include <stdio.h>

#define PRN3(x,y,z) printf(#x " has value %.1f and " #y " has value %.1f and " #z " has value %.1f\n", x,y,z)

int main(void){ 
    printf("%c %c %c %c\n", tolower('a'), _tolower('a'), toupper('a'), _toupper('a'));
    return 0;
}
