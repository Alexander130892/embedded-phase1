/*
 * File:    Ex_18_strcpy_y.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program demonstrates the dangerous behavior of strcpy when
 *   source and destination strings overlap, copying the entire
 *   alphabet string "abcdefghijklmnopqrstuvwxyz" over itself starting
 *   from 3 characters before the end, resulting in undefined behavior
 *   due to overlapping memory regions.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Consts
#define N 5

//Typedefs

//Prototype Functions


//MAIN
int main(){
    char a[]="abcdefghijklmnopqrstuvwxyz";
    char*p=a;
    char *q=a+strlen(a)-3;
    
    printf("a=%s\n", a);
    strcpy(q,p);
    printf("a=%s\n", a);
    return EXIT_SUCCESS;
}
