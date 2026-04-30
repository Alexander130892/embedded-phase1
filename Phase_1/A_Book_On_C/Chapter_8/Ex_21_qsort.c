/*
 * File:    Ex_21_qsort.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This C program demonstrates the qsort() function by generating
 *   random arrays of characters and floats, then sorting them using
 *   custom comparison functions - characters are sorted
 *   lexicographically, while floats are sorted first by their
 *   fractional parts and then by their decimal (integer) parts.
 */
//write pack into one line

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define M 32
#define N 11
#define fractional_part(x)  (x - (int) x)
#define decimal_part(x)  ((int) x)
#define random_char()       (rand()%26+'a')
#define random_float()      (rand()%100 /10.0)

#define FILL(array, sz, type)   \
    if(strcmp(type, "char")==0) \
        for(i=0;i<sz;i++)       \
            array[i]= random_char();\
    else                        \
        for(i=0;i<sz;i++)       \
            array[i]= random_float();\

#define PRINT(array, sz, cntrl_string) \
        for (i = 0 ; i < sz ; i++)      \
            printf(cntrl_string, array[i]);\
        putchar('\n');

int compare_fractional_part(const void*, const void*);
int compare_decimal_part(const float *p,const float *q);
int lexico(const void*, const void*);


int main(void){ 
    char a[M];
    float b[N];
    int i;

    srand(time(NULL));
    FILL(a, M, "char");
    PRINT(a, M, "%-2c");
    qsort(a, M, sizeof(char), lexico);
    PRINT(a, M, "%-2c");
    printf("----------\n");

    FILL(b, N, "float");
    PRINT(b, N, "%-6.1f");
    qsort(b,N, sizeof(float), compare_fractional_part);
    PRINT(b, N, "%-6.1f");
    qsort(b,N, sizeof(float), compare_decimal_part);
    PRINT(b, N, "%-6.1f");
    return 0;
}

int compare_decimal_part(const float *p, const float *q)
{
    float x;
    x = decimal_part(*p) - decimal_part(*q);
    return ((x==0.0)?0:((x<0.)?-1:1));
}
int compare_fractional_part(const void* vp, const void* vq)
{
    const float *p=vp, *q=vq;
    float x;
    x = fractional_part(*p) - fractional_part(*q);
    return ((x==0.0)?0:((x<0.)?-1:1));
}
int lexico(const void* vp, const void* vq){
    const char *p=vp, *q=vq;
    return (*p - *q);
}