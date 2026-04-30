/*
 * File:    merge.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements the merge function for mergesort, which
 *   combines two sorted arrays `a` and `b` of sizes `m` and `n`
 *   respectively into a single sorted array `c`. The function
 *   maintains the sorted order by comparing elements from both input
 *   arrays and copying any remaining elements after one array is
 *   exhausted.
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