/*
 * File:    Ex_17_p_incr.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates the difference between pointer increment
 *   (`*p++`) and value increment (`(*p)++`) by first printing
 *   characters from a string while advancing the pointer, then
 *   printing and incrementing each character's ASCII value in place,
 *   showing how the original string gets modified in the second case.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>

//Consts
#define N 5

//Typedefs

//Prototype Functions


//MAIN
int main(){
    char a[]="abc";
    char*p;
    int i;
    
    p=a;
    for(i=0;i<3;i++){
        printf("%c\n", *p++);
    }
    printf("a=%s\n", a);
    p =a;
    for(i=0;i<3;i++){
        printf("%c\n", (*p)++);
    }
    printf("a=%s\n", a);

    return EXIT_SUCCESS;
}
