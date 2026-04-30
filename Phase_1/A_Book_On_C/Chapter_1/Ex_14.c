/*
 * File:    Ex_14.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program demonstrates pointer arithmetic and string
 *   manipulation in C by printing character values derived from
 *   pointer operations and modifying a substring to replace 'e' with
 *   'E' and spaces with newlines. It showcases various ways to access
 *   and manipulate characters in strings using both array notation and
 *   pointer dereferencing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING 100

int main(void){
    
    char c='a', *p, s[MAXSTRING];
    p=&c;
    printf("%c%c%c ", *p, *p+1, *p+2);
    strcpy(s,"ABC");
    printf("%s, %c%c%s\n",s,*s+6,*s+7,s+1);
    strcpy(s, "she sells sea shells by the seashore");
    p=s+14;
    for( ; *p != '\0' ; ++p){
        if(*p =='e')
            *p = 'E';
        if(*p == ' ')
            *p = '\n';
    }
    for( ; *p != '\0' ; ++p)
        if(*p =='e')
            *p = 'E';
        else if(*p == ' ')
            *p = '\n';
    
    printf("%c\n",*(s+1));

   
    return EXIT_SUCCESS;
}