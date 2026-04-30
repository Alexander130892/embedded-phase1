/*
 * File:    Ex_35_string_matrix.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates complex pointer arithmetic and
 *   dereferencing operations on a 2D array of string pointers,
 *   printing individual characters accessed through various
 *   multi-level pointer expressions. The program serves as an exercise
 *   in understanding how to navigate and extract data from nested
 *   pointer structures containing strings.
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