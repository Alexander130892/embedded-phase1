/*
 * File:    Ex_18_strcpy_y.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program demonstrates overlapping memory behavior with
 *   strcpy by copying the beginning of a string
 *   ("abcdefghijklmnopqrstuvwxyz") to a position near its end,
 *   starting 3 characters from the end, which results in undefined
 *   behavior due to the overlapping source and destination memory
 *   regions.
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
