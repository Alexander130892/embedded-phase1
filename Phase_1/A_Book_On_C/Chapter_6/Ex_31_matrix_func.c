/*
 * File:    Ex_31_matrix_func.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates passing a 3x3 integer matrix to a function
 *   and prints specific matrix elements (a[1][0], -a[1][1], a[0][0],
 *   a[2][2]) followed by ".... infinity". The program serves as an
 *   example of matrix function parameter passing in C, showing two
 *   equivalent syntax forms for array parameters.
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

