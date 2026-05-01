/*
 * File:    Ex_35_string_matrix.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates complex pointer dereferencing with a 2D
 *   array of strings by printing individual characters accessed
 *   through various nested pointer expressions. It serves as an
 *   educational example showing different ways to navigate and access
 *   elements within a matrix of string pointers using multiple levels
 *   of indirection.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Consts

//Typedefs

//Prototype Functions
void print_arg(const char *s, int cap);

//MAIN
int main(){
    char* p[2][3]={"abc","defg","hi",
                    "jklmno", "pqrstu","vwxyz"};

    printf("%c\n",***(p)) ;    
    printf("%c\n",**(p[1])) ;
    printf("%c\n",**(p[1]+2)) ;
    //printf("%c\n",*(*(p+1)+1)[7]) ;
    printf("%c\n",(*(*(p+1)+1))[7]) ;
    printf("%c\n",*(p[1][2]+2)) ;
    return 0;
}