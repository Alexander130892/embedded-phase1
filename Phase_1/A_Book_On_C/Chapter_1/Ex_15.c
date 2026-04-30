/*
 * File:    Ex_15.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program demonstrates string manipulation by copying two parts
 *   of a nursery rhyme into separate strings, printing them using both
 *   printf and a custom character-by-character printing function, then
 *   concatenating them together and displaying the result. The custom
 *   prn_string function iterates through each character of a string
 *   using pointer arithmetic to print it one character at a time.
 */

#include <stdio.h>
#include <string.h>

#define MAXSTRING 100

void prn_string(char *);

int main(void){
    char s1[MAXSTRING], s2[MAXSTRING];
    strcpy(s1, "Mary, Mary, quite contrary, \n");
    strcpy(s2, "how does your garden grow?\n");
    printf("Original:\n");
    printf("%s",s1);
    printf("%s",s2);
    printf("Func:\n");
    prn_string(s1);
    prn_string(s2);
    strcat(s1,s2);
    printf("Original:\n");
    printf("%s",s1);
    printf("Func:\n");
    prn_string(s1);
    
}

void prn_string(char * str){
    for(;*str!='\0';str++){
        putchar(*str);
    }
}