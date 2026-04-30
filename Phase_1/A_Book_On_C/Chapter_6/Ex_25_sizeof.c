/*
 * File:    Ex_25_sizeof.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates the sizeof operator in C by printing the
 *   sizes of different data types including string arrays, pointers,
 *   integer arrays, and double arrays, while also showing how array
 *   parameters are treated as pointers in function calls. The file
 *   includes additional unused functions for arithmetic operations on
 *   arrays representing large numbers stored as digit arrays.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

//Consts
#define N 40

//Typedefs
typedef const char cchar;

//Prototype Functions
void add(int *, int*, int *);
void wrt(cchar *s, int *);
void mul(int *result, int *a, int *b);

void f(int *a){
    printf("in f() sizeof(a)= %lu\n", sizeof(a));
}

//MAIN
int main(){
    char s[]="deep in the heart of texas";
    char *p = "deep in the heart of texas";
    int a[3];
    double d[5];
    printf("%s%lu\n%s%lu\n%s%lu\n%s%lu\n",
            "sizeof(s)= ", sizeof(s),
            "sizeof(p)= ", sizeof(p),
            "sizeof(a)= ", sizeof(a),
            "sizeof(d)= ", sizeof(d));
    f(a);
    return EXIT_SUCCESS;
}

void add(int * sum, int* a, int *b){
    int carry=0;
    int i;

    for(i=0; i<N;i++){
        sum[i]=a[i]+b[i]+carry;
        if(sum[i]<10)
            carry=0;
        else{
            carry=sum[i]/10;
            sum[i] %= 10;
        }
    }
}

void mul(int *result, int *a, int *b){
    memset(result, 0, 2*N * sizeof(int));
    for(int i = 0; i < N; i++){
        int carry = 0;
        for(int j = 0; j < N; j++){
            result[i+j] += a[i] * b[j] + carry;
            carry        = result[i+j] / 10;
            result[i+j] %= 10;
        }
        result[i+N] += carry;   /* propagate final carry past inner loop */
    }
}
void wrt(cchar *s, int *a){
    int i;
    printf("%s", s);
    // print leading zeros as blanks
    for(i=N-1;i>0 && a[i]==0;i--){
        putchar(' ');
    }
    //remaining digits
    for(;i>=0;--i){
        printf("%d", a[i]);
    }
    putchar('\n');
}