/*
 * File:    Ex_31_matrix_func.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program demonstrates passing a 2D array to a function by
 *   initializing a 3x3 integer matrix and printing selected elements
 *   from it using pointer notation for array parameters.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <ctype.h>

//Consts
#define CYPHER (3)

//Typedefs
typedef const char cchar;

//Prototype Functions

void try_me(int [][3]);


//MAIN
int main(){
    int a[3][3]={{2,5,7},{0,-1,2},{7,9,3}};
    try_me(a);
    return EXIT_SUCCESS;
}

void try_me(int (*a)[3]){
    printf("%d %d %d %d .... infinity\n",
        a[1][0], -a[1][1], a[0][0], a[2][2]);
}

