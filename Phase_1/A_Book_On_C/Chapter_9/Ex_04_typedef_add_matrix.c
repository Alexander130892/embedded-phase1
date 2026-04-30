/*
 * File:    Ex_04_typedef_add_matrix.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This C program demonstrates matrix addition using a custom typedef
 *   for a 5x5 integer matrix, adding two predefined 3x3 matrices
 *   element-wise and printing the result. The program uses typedef to
 *   create a convenient matrix data type and includes functions for
 *   matrix addition and printing.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define ROWS 5 //rows
#define COLLUMNS 5 // collumns

typedef int matrix[ROWS][COLLUMNS];

//Typedefs
//typedef char* string;

void print_matrix( matrix matr)
{
    for(int row = 0 ; row < ROWS ; row++ )
    {
        for(int col = 0 ; col < COLLUMNS ; col++)
        {
            printf("%d ", matr[row][col]);
        }
        putchar('\n');
    }
}

void add_matrix(matrix a, matrix b, matrix c)
{
    for(int row = 0 ; row < ROWS ; row++ )
    {
        for(int col = 0 ; col < COLLUMNS ; col++)
        {
            c[row][col]=a[row][col]+b[row][col];
        }
    }
    return;
}

int main(){
    matrix a = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    matrix b = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    matrix c={0};
    add_matrix(a,b,c);
    print_matrix(c);
    
    return EXIT_SUCCESS;
}
