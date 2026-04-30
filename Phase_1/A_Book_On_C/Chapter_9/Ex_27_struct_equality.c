/*
 * File:    Ex_27_struct_equality.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This file demonstrates struct assignment and the fact that direct
 *   equality comparison between struct instances is not allowed in C,
 *   showing that while structs can be assigned to each other (copying
 *   all members), they cannot be compared using the == operator.
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
