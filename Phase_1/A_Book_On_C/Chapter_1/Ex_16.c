/*
 * File:    Ex_16.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file reads characters from standard input one by one using
 *   scanf and immediately prints each character to standard output,
 *   effectively creating a character-by-character echo program that
 *   continues until input fails or EOF is reached.
 */

#include <stdio.h>
#include <string.h>

int main(void){
    char c;
    while(scanf("%c",&c)==1){
            printf("%c",c);

    }
       
}
