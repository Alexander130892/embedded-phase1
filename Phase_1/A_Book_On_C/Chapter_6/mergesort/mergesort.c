/*
 * File:    mergesort.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a recursive merge sort algorithm that sorts
 *   an integer array by recursively dividing it at the largest power
 *   of 2 less than the array size, then merging the sorted subarrays
 *   back together. The implementation includes two commented-out
 *   iterative versions and uses dynamic memory allocation for the
 *   temporary merge buffer.
 */

#include "mergesort.h"
// void merge_sort(int key[], int n){
//     int j, k, m, *w;
//     for(m=1;m<n;m*=2);
//     if (n<m){
//         printf("ERROR: array size not a power of 2");
//         exit(1);
//     }
//     w=(int*)calloc(n, sizeof(int));
//     assert(w!=NULL && "Memory allocation Failed");

//     for(k=1;k<n;k *=2){
//         for(j=0;j<n-k;j+=2*k){
//             //merge two subarrays of key[] into a subarray of w[]
//             merge(key+j, key+j+k,w+j, k, k);
//         }
//         for(j=0; j<n;j++)
//             key[j]=w[j];
//     }
//     free(w);
// }


// void merge_sort(int key[], int n){
//     int j, k, *w;
//     w = (int*)calloc(n, sizeof(int));
//     assert(w != NULL && "Memory allocation failed");

//     for(k = 1; k < n; k *= 2){
//         int merged_end = 0;
//         for(j = 0; j < n-k; j += 2*k){
//             int right_size = (j + 2*k <= n) ? k : n - j - k;
//             merge(key+j, key+j+k, w+j, k, right_size);
//             merged_end = j + k + right_size;
//         }
//         /* copy merged region back; tail [merged_end..n-1] stays untouched */
//         for(j = 0; j < merged_end; j++)
//             key[j] = w[j];
//     }
//     free(w);
// }

//Recursive version
void merge_sort(int key[], int n){
    if(n <= 1) return;

    /* largest power of 2 strictly less than n */
    int m = 1;
    while(m * 2 < n) m *= 2;

    merge_sort(key,     m);      /* sort left  [0..m-1] */
    merge_sort(key + m, n - m);  /* sort right [m..n-1] */

    int *w = (int*)malloc(n * sizeof(int));
    assert(w != NULL && "Memory allocation failed");
    merge(key, key + m, w, m, n - m);
    for(int i = 0; i < n; i++) key[i] = w[i];
    free(w);
}