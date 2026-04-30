/*
 * File:    Ex_27_struct_equality.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates that while you can assign one struct to
 *   another in C (copying all member values), you cannot directly
 *   compare structs for equality using the == operator, which would
 *   result in a compilation error.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
typedef struct{
    int a;
    int b;
}test;

int main(){
    test A={1,3};
    test B=A;
    printf("%d %d\n",A.b, B.b );
    //printf("%d\n",A==B ); //error
    return EXIT_SUCCESS;
}
