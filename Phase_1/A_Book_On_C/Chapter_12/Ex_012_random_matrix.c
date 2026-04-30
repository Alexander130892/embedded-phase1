/*
 * File:    Ex_012_random_matrix.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This program creates a 3x4 integer matrix, fills it with random
 *   values between -5 and 5, prints the matrix to the console, and
 *   then frees the allocated memory. The matrix allocation appears to
 *   have a bug where rows and columns are swapped during memory
 *   allocation but accessed correctly during filling and printing.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <math.h>

void fill_matrix(int ** matrix, int n_rows, int n_cols, int limit);
void print_matrix(int ** matrix, int n_rows, int n_cols);
void free_matrix(int **matrix, int n_cols);

int main(void)
{
    int n_rows=3;
    int n_cols=4;
    int ** matrix = calloc(n_cols, sizeof(int*));
    for(int index = 0; index < n_cols ; index++){
        matrix[index] = calloc(n_rows, sizeof(int));
    }
    fill_matrix(matrix, n_rows, n_cols, 5);
    print_matrix(matrix, n_rows, n_cols);
    free_matrix(matrix, n_cols);
    return 0;
}

void fill_matrix(int ** matrix, int n_rows, int n_cols, int limit){
    srand(time(NULL));
    for(int row =0; row < n_rows ; row++){
        for(int col = 0; col < n_cols; col++){
            matrix[row][col] = rand() % (2*limit+1) -limit ;
        }
    }
}
void free_matrix(int **matrix, int n_cols) {
    for (int i = 0; i < n_cols; i++)
        free(matrix[i]);
    free(matrix);
}

void print_matrix(int ** matrix, int n_rows, int n_cols){
    for(int row =0; row < n_rows ; row++){
        for(int col = 0; col < n_cols; col++){
            printf("%d\t",matrix[row][col]);
        }
        putchar('\n');
    }
}
