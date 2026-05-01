/*
 * File:    Ex_14.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates pointer arithmetic and string manipulation
 *   in C by performing character operations on variables and strings,
 *   including incrementing ASCII values and replacing characters in a
 *   substring starting from position 14 of "she sells sea shells by
 *   the seashore". The program prints various outputs showing pointer
 *   dereferencing, ASCII arithmetic results, and modified string
 *   content where 'e' becomes 'E' and spaces become newlines.
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