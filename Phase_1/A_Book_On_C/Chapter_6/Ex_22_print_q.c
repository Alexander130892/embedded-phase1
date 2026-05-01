/*
 * File:    Ex_22_print_q.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program prints "Hello!" by using clever pointer arithmetic
 *   and array indexing on string literals to extract individual
 *   characters, and includes an unused palindrome checking function
 *   that ignores whitespace and case.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Consts
#define N 5

//Typedefs

//Prototype Functions
int is_palindrome(char*);

//MAIN
int main(){
    printf("%c%c%c%c%c!\n", "ghi"[1], *("def"+1), *"abc"+11, "klm"[1], *"ghi"+8);
    return EXIT_SUCCESS;
}

int is_palindrome(char* str){
    char *l= &str[0];
    while(*l != '\0')l++; //find end of string
    char *r=l-1;
    l= &str[0];
    while(l<r){
        while(l < r && isspace(*l)) l++;
        while(l < r && isspace(*r)) r--;
        if(tolower(*l++) != tolower(*r--))
            return 0;
    }
    return 1;
}