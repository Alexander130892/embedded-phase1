/*
 * File:    Ex_27_struct_equality.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates that while struct assignment is valid in C
 *   (copying all members from one struct to another), direct struct
 *   equality comparison using the == operator is not supported and
 *   will cause a compilation error.
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
