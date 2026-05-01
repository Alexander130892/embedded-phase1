/*
 * File:    merge.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file implements the merge function that combines two sorted
 *   arrays into a single sorted array by comparing elements from both
 *   arrays and copying them in ascending order to the destination
 *   array.
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