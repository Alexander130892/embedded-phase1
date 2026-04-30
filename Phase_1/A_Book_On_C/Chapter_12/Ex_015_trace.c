/*
 * File:    Ex_015_trace.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program dynamically allocates a 3x3 matrix of doubles
 *   (initialized to zero) and calculates its trace by summing the
 *   diagonal elements using a dedicated trace function. The program
 *   demonstrates dynamic memory allocation for a 2D matrix using an
 *   array of pointers and properly deallocates the memory before
 *   terminating.
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