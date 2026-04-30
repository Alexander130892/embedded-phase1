/*
 * File:    Ex_20_pallindrome.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program reads strings from standard input and counts how
 *   many of them are palindromes, ignoring whitespace and case
 *   differences. It outputs the total count of palindromes found with
 *   the message "palindromes in directory".
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
    char s[256];
    int cnt=0;
    while(scanf("%s", s)==1)
        if(is_palindrome(s)) cnt++;
    
    printf("%d palindromes in directory\n", cnt);
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