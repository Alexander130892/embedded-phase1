/*
 * File:    strcat_.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a custom string concatenation function
 *   `strcat_` that appends the second string to the end of the first
 *   string, modifying the first string in place. The program
 *   demonstrates this functionality by concatenating two strings and
 *   printing the results before and after the operation.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

//Consts

//Typedefs

//Prototype Functions
void strcat_(char *s1, const char* s2); 

int main(){
    char string1[100] = "I was here first";
    const char * string2 = ", but then I came";
    printf("%s\n%s\n", string1, string2);
    strcat_(string1, string2);
    printf("%s\n%s\n", string1, string2);
}

void strcat_(char *s1, const char* s2){
    while((*s1)!='\0')
        s1++;//arrived at end of s1
    while((*s1++=*s2++));
    return;
}
