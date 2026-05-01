/*
 * File:    Ex_18_strcpy_y.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates the dangerous behavior of using strcpy()
 *   with overlapping memory regions by copying the entire alphabet
 *   string to a position near its end, resulting in undefined behavior
 *   and memory corruption.
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
