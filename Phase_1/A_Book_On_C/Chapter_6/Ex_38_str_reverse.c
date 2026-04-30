/*
 * File:    Ex_38_str_reverse.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates string reversal by implementing a function
 *   that reverses a string in-place using two pointers that swap
 *   characters from both ends moving toward the center. The program
 *   prints the original string, the reversed string, and shows that
 *   the original string has been permanently modified.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <limits.h>

//Consts
#define COUNT 1000000.0

//Typedefs

//Prototypes
char * reverse(char*);

int main() {
    char s[] = "abcdefghijklmnopqrstuvwxyz";
    printf("%s\n", s);
    printf("%s\n", reverse(s));
    printf("%s\n", s);
    return 0;
}

char * reverse(char*s){
    char *p, *q, tmp;
    int n=strlen(s);
    q = (n > 0) ? s + n - 1: s;
    for(p=s ; p < q; p++, q--)
    {
        tmp = *p;
        *p = *q;
        *q = tmp;
    }
    return s;
}