/*
 * File:    Ex_010_trace.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This C program calculates and prints the trace (sum of diagonal
 *   elements) of a 3x3 matrix that is dynamically allocated as an
 *   array of pointers to double arrays. The matrix is initialized with
 *   zeros using calloc, so the trace will always be 0.0, and the
 *   program properly frees the allocated memory before terminating.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

double trace(double **, int);

int main(void)
{
    int     i;
    double *    a[3];
    double det;


    for(i = 0; i < 3; i++){
        a[i]=calloc(3, sizeof(double));
    }
    double tr = trace(a, 3);

    printf("trace = %.1lf\n", tr);
    for(i = 0; i < 3; i++){
        free(a[i]);
    }


    return 0;
}

//a dynamically allocated matrix is defined as a pointer to pointers --> memory is not contigous
//double ** would not work on static matrices (contigous), which would need matrix[][3] to be able to make the mapping

double trace(double ** matrix, int N){
    double val=0;
    for (int index = 0 ; index < N ; index ++){
        val+= matrix[index][index];
    }
    return val;

}