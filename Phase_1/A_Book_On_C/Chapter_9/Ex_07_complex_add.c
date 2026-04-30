/*
 * File:    Ex_07_complex_add.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This file demonstrates complex number addition using a custom
 *   complex struct, implementing the operation through both
 *   pass-by-value and pass-by-reference functions. It creates two
 *   complex numbers, adds them using both methods, and displays the
 *   results to show different parameter passing techniques in C.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define PI 3.14 //rows
#define COLLUMNS 5 // collumns

typedef struct{
    double real;
    double imaginary;
} complex;

complex complex_add_value(complex a, complex b);
void complex_add_ref(complex *a, complex *b, complex *result);

int main(){
    complex a = {2,3};
    complex b;
    b.real = 4;
    b.imaginary = -2;
    complex c={0};
    c=complex_add_value(a,b);
    printf("%.1f +i %.1f + %.1f + i %.1f = %.1f + i %.1f\n", a.real, a.imaginary, b.real, b.imaginary, c.real, c.imaginary);

    complex d={0};
    complex_add_ref(&a,&b,&d);
    printf("%.1f +i %.1f + %.1f + i %.1f = %.1f + i %.1f\n", a.real, a.imaginary, b.real, b.imaginary, d.real, d.imaginary);

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