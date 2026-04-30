/*
 * File:    Ex_05_concurrent_matrix_multiplication copy.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This file implements concurrent matrix multiplication by forking N
 *   child processes, where each child computes one row of the result
 *   matrix and communicates it back to the parent process through
 *   pipes. The program multiplies two 4x4 matrices (A and an identity
 *   matrix B) using parallel processing to demonstrate inter-process
 *   communication.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


//Consts
#define N 4

//Typedefs

//Prototype Functions
void init_matrices(int A[N][N], int B[N][N]);
void print_matrix(const char *label, int M[N][N]);

//MAIN
int main(void)
{
    int A[N][N], B[N][N], C[N][N];
    int pipes[N][2];       /* pipes[i][0] = read end, [1] = write end */

    init_matrices(A, B);
    print_matrix("A", A);
    print_matrix("B", B);

    /* Create all pipes before forking */
    for (int i = 0; i < N; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe failed");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            return EXIT_FAILURE;
        }

        if (pid == 0) {                          /* child */
            close(pipes[i][0]);                  /* close read end */

            int row[N];
            for (int j = 0; j < N; j++) {
                row[j] = 0;
                for (int k = 0; k < N; k++)
                    row[j] += A[i][k] * B[k][j];
            }

            write(pipes[i][1], row, sizeof(row));
            close(pipes[i][1]);
            exit(EXIT_SUCCESS);
        }

        /* Parent: close write end — only child should write */
        close(pipes[i][1]);
    }

    for (int i = 0; i < N; i++) {
        read(pipes[i][0], C[i], sizeof(C[i]));
        close(pipes[i][0]);
    }

    print_matrix("C = A x B", C);

    return EXIT_SUCCESS;
}

//Helper-Functions

void init_matrices(int A[N][N], int B[N][N])
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i * N + j + 1;
            B[i][j] = (i == j) ? 1 : 0;
        }
    }
}

void print_matrix(const char *label, int M[N][N])
{
    printf("\n%s:\n", label);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%5d", M[i][j]);
        putchar('\n');
    }
}