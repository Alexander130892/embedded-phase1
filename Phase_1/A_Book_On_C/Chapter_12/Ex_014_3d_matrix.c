/*
 * File:    Ex_014_3d_matrix.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This file demonstrates dynamic allocation and manipulation of a 3D
 *   matrix in C, creating a 2x3x4 integer array that is filled with
 *   random values between -5 and 5, printed to the console, and then
 *   properly deallocated. The program showcases memory management
 *   techniques for multi-dimensional arrays using triple pointers and
 *   nested loops.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <math.h>

void fill_matrix(int *** matrix, int d, int n_rows, int n_cols, int limit);
void print_matrix(int *** matrix, int d, int n_rows, int n_cols);
void free_matrix(int *** matrix, int d, int n_cols);

int main(void)
{
    int i, j , n1=2, n2=3, n3=4;
    int *** a;

    a=calloc(n1, sizeof(int **));
    for(i=0; i< n1; i++){
        a[i]=calloc(n2, sizeof(int *));
        for(j=0; j<n2;j++){
            a[i][j]=calloc(n3, sizeof(int));
        }
    }
    fill_matrix(a, n1, n2, n3, 5);
    print_matrix(a, n1, n2, n3);
    free_matrix(a, n1, n2);

    return 0;
}

void fill_matrix(int *** matrix, int d, int n_rows, int n_cols, int limit){
    srand(time(NULL));
    for(int i=0; i<d; i++){
        for(int row =0; row < n_rows ; row++){
            for(int col = 0; col < n_cols; col++){
                matrix[i][row][col] = rand() % (2*limit+1) -limit ;
            }
        }
}}

void free_matrix(int *** matrix, int n1, int n2){
    for(int dim=0; dim < n1; dim++){
        for(int i = 0; i<n2; i++){
            free(matrix[dim][i]);
        }
        free(matrix[dim]);
    }
}

void print_matrix(int *** matrix, int d, int n_rows, int n_cols){
    for(int i=0; i<d; i++){
        for(int row =0; row < n_rows ; row++){
            for(int col = 0; col < n_cols; col++){
                printf("%d\t",matrix[i][row][col]);
            }
            putchar('\n');
        }
        printf("\n");
    }
    
}   

