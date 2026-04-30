/*
 * File:    Ex_46_remove_vowel.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file reads characters from standard input and outputs only
 *   the non-vowel characters, effectively filtering out all vowels (a,
 *   e, i, o, u, y) in a case-insensitive manner. It treats 'y' as a
 *   vowel and adds a newline at the end of the filtered output.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Consts

//Typedefs

//Prototype Functions
int is_vowel(char c);

//MAIN
int main(void){
    char c;
    while((c=getchar())!=-1){
        if(!is_vowel(c))
            putchar(c);
    }
    putchar('\n');
    return EXIT_SUCCESS;
}

int is_vowel(char c){
    
    switch(tolower(c)){
        case 'a':
            return 1;
        case 'e':
            return 1;
        case 'i':
            return 1;
        case 'o':
            return 1;
        case 'u':
            return 1;
        case 'y':
            return 1;
        default:
            return 0;
    }
}