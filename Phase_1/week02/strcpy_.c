/*
 * File:    strcpy_.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements a custom version of the standard C library
 *   strcpy function that copies a string from source to destination
 *   and returns a pointer to the destination. The implementation
 *   demonstrates the function by copying one string over another and
 *   printing the results before and after the operation.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

//Consts

//Typedefs

//Prototype Functions
char* strcpy_(char *s1, const char* s2); //he real strcpy returns char * (the original destination pointer) rather than void, for the same reason memset does: it allows chaining:

int main(){
    char string1[100] = "I was here first";
    const char * string2 = ", but then I came";
    printf("%s\n%s\n", string1, string2);
    strcpy_(string1, string2);
    printf("%s\n%s\n", string1, string2);
}

char *strcpy_(char *s1, const char *s2){
    char *start = s1;
    while((*s1++ = *s2++));
    return start;  // return original destination
}