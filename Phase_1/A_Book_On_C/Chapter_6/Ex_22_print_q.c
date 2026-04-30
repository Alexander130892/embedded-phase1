/*
 * File:    Ex_22_print_q.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file contains a palindrome checking function but its main
 *   purpose is to demonstrate complex C string indexing and pointer
 *   arithmetic by printing "hello!" through various string access
 *   methods. The palindrome function is defined but never called from
 *   main.
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