/*
 * File:    Ex_23_largenumbers.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program performs arithmetic operations (addition and
 *   multiplication) on large integers that are too big to fit in
 *   standard integer types by representing them as arrays of
 *   individual digits. It demonstrates adding two large numbers and
 *   multiplying two large numbers, then displays the formatted results
 *   with proper digit alignment.
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

//MAIN
int main(){
    int a[N]={7,5,9,8,9,7,5,0,0,9,9,0,8,8};
    int b[N]={7,7,5,3,1,2,8,8,9,6,7,7};
    int c[N]={3,7};
    int sum[N];
    int prod[2*N];
    int ndigits;


    add(sum, a, b);
    mul(prod, b, c);
    wrt("Integer a: ", a);
    wrt("Integer b: ", b);
    wrt("Integer c: ", c);
    wrt("      Sum: ", sum);
    wrt("  Product: ", prod);
    putchar('\n');
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