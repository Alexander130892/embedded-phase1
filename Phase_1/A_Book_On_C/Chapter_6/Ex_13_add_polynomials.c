/*
 * File:    Ex_13_add_polynomials.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program adds two polynomials represented as coefficient
 *   arrays and stores the result in a third array, then prints the
 *   resulting polynomial coefficients. It also includes a Horner's
 *   method function for polynomial evaluation, though this function is
 *   not used in the main program.
 */
 //Libraries
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define N 5


//Typedefs

//Prototype
void add_p(double*, double*, double*, int);
double eval_hor(double*, double, int);

int main() {
    double g[N+1]={1,2,3,4,5,6};
    double h[N+1]={6,5,4,3,2,1};
    double f[N+1]={0};
    add_p(f,g,h,N);
    for(int i=0;i<N+1;i++){
        printf("%.0f,",f[i]);
    }
    return 0;
}
void add_p(double* f, double* g, double* h, int K){
    for(int i=0;i<N+1;i++){
        f[i]=g[i]+h[i];
    }
}
double eval_hor(double *p, double x, int K) {
    double val = p[K];
    for (int n = K - 1; n >= 0; n--)
        val = val * x + p[n];
    return val;
}

