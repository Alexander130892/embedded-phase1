/*
 * File:    swap.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This file demonstrates swapping two integer values using an
 *   XOR-based bitwise operation that avoids using temporary variables.
 *   The swap function includes a guard against self-assignment and
 *   uses three XOR operations to exchange the values of two integers
 *   passed by reference.
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
