/*
 * File:    Ex_14.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program demonstrates pointer arithmetic and string
 *   manipulation by performing character operations on strings,
 *   including printing characters with ASCII offsets and replacing
 *   specific characters ('e' with 'E' and spaces with newlines) in a
 *   substring starting from position 14 of "she sells sea shells by
 *   the seashore". The program showcases various pointer dereferencing
 *   techniques and string traversal methods.
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