/*
 * File:    merge.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements the merge operation for merge sort, combining
 *   two sorted arrays `a` and `b` of sizes `m` and `n` respectively
 *   into a single sorted array `c`. The function uses a two-pointer
 *   technique to efficiently merge the arrays in ascending order,
 *   handling remaining elements from either array after the main
 *   comparison loop.
 */
#include "mergesort.h"

void merge(int a[], int b[], int c[], int m, int n){
    int i=0, j=0, k=0;

    while(i<m && j<n){
        if(a[i]<b[j])
            c[k++]=a[i++];
        else
            c[k++]=b[j++];
    }
    while(i<m)
        c[k++]=a[i++];
    while(j<n)
        c[k++]=b[j++];
}