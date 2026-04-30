/*
 * File:    mergesort.h
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This header file declares functions for implementing the merge
 *   sort algorithm, including the main sorting function, a merge
 *   helper function for combining sorted arrays, and a utility
 *   function for printing arrays.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int a[], int b[], int c[], int m, int n);
void merge_sort(int key[], int n);
void print_arr(int key[], int sz);
