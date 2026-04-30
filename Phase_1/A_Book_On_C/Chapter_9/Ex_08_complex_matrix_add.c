/*
 * File:    Ex_08_complex_matrix_add.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This C program performs addition of two 3x5 matrices containing
 *   complex numbers (with real and imaginary parts) and displays the
 *   result. The program defines a complex number structure, implements
 *   matrix addition by adding corresponding complex elements, and
 *   includes functions to print matrices and add complex numbers.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define ROWS 3 //rows
#define COLLUMNS 5 // collumns

typedef struct{
    double real;
    double imaginary;
} complex;

typedef complex matrix[ROWS][COLLUMNS];
complex complex_add_value(complex a, complex b);
void print_matrix( matrix matr)
{
    for(int row = 0 ; row < ROWS ; row++ )
    {
        for(int col = 0 ; col < COLLUMNS ; col++)
        {
            printf(" %.1f + i %.1f ", matr[row][col].real, matr[row][col].imaginary );
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
            c[row][col]=complex_add_value(a[row][col],b[row][col]);
        }
    }
    return;
}

int main(){
    matrix a={{2,2}};
    matrix b = {{1,1},{2,2}};
    matrix c = {0};
    print_matrix(a);
    add_matrix(a,b,c);
    putchar('\n');
    print_matrix(c);

    return EXIT_SUCCESS;
}

complex complex_add_value(complex a, complex b)
{
    complex result={0};
    result.real = a.real + b.real;
    result.imaginary = a.imaginary + b.imaginary;
    return result;
}
void complex_add_ref(complex *a, complex *b, complex *result){
    result->real = a->real + b->real;
    result->imaginary = a->imaginary + b->imaginary;
    return;
}