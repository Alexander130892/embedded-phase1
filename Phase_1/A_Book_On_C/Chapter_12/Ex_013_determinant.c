/*
 * File:    Ex_013_determinant.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This file creates a 3x3 matrix filled with random integers and
 *   provides functionality to calculate its determinant using Gaussian
 *   elimination with partial pivoting. However, the main function only
 *   fills and prints the matrix without actually calling the
 *   determinant function.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <math.h>

void fill_matrix(int ** matrix, int n_cols, int n_rows, int limit);
void print_matrix(int ** matrix, int n_cols, int n_rows);
void free_matrix(int **matrix, int n_cols);
int determinant(int **matrix, int N);

int main(void)
{
    int n_rows=3;
    int n_cols=3;
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


int determinant(int **matrix, int N) {
    /* Gaussian elimination on a floating-point copy to avoid
     * integer division truncation during elimination steps.    */
    double **m = malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        m[i] = malloc(N * sizeof(double));
        for (int j = 0; j < N; j++)
            m[i][j] = (double)matrix[i][j];
    }

    int sign = 1;

    for (int col = 0; col < N; col++) {

        /* --- Partial pivoting: find the largest absolute value in
         *     this column at or below the diagonal.              --- */
        int pivot = col;
        for (int row = col + 1; row < N; row++)
            if (fabs(m[row][col]) > fabs(m[pivot][col]))
                pivot = row;

        if (m[pivot][col] == 0.0) {         /* Singular matrix      */
            for (int i = 0; i < N; i++) free(m[i]);
            free(m);
            return 0;
        }

        if (pivot != col) {                  /* Swap rows            */
            double *tmp = m[col];
            m[col]      = m[pivot];
            m[pivot]    = tmp;
            sign        = -sign;             /* Each swap flips sign */
        }

        /* --- Eliminate entries below the pivot row              --- */
        for (int row = col + 1; row < N; row++) {
            double factor = m[row][col] / m[col][col];
            for (int k = col; k < N; k++)
                m[row][k] -= factor * m[col][k];
        }
    }

    /* Product of the diagonal of the upper-triangular result */
    double det = 1.0;
    for (int i = 0; i < N; i++)
        det *= m[i][i];

    for (int i = 0; i < N; i++) free(m[i]);
    free(m);

    return (int)round(det * sign);
}