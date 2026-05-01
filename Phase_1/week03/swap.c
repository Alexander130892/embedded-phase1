/*
 * File:    swap.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates swapping two integer values using a bitwise
 *   XOR algorithm that exchanges values without requiring a temporary
 *   variable. The swap function includes a safety check to prevent
 *   issues when both pointers reference the same memory location.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//Consts

//Typedefs

//Prototype Functions
void swap(int * a, int * b);
//MAIN 
int main(){
    int a = 3;
    int b = 9;
    printf("Before swap: %d - %d\n", a,b);
    swap(&a, &b);
    printf("After swap: %d - %d\n", a,b);
    return EXIT_SUCCESS;
}

void swap(int * a, int * b){
    if (a == b) return;   
    *a = ((*a)^(*b));
    *b = ((*b)^(*a));
    *a = ((*a)^(*b));
    return;
}
