/*
 * File:    Ex_16.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file reads characters one by one from standard input and
 *   immediately prints each character to standard output, effectively
 *   creating a character-by-character echo program that continues
 *   until EOF is encountered.
 */
#include <stdio.h>
#include <string.h>

int main(void){
    char c;
    while(scanf("%c",&c)==1){
            printf("%c",c);

    }
       
}
