/*
 * File:    mergesort.h
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This header file declares the interface for a merge sort
 *   implementation, including functions to sort an integer array,
 *   merge two sorted arrays, and print an array.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int a[], int b[], int c[], int m, int n);
void merge_sort(int key[], int n);
void print_arr(int key[], int sz);
