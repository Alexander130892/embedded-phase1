/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates a merge sort implementation by sorting a
 *   predefined array of 15 integers and displaying the array contents
 *   before and after sorting.
 */

#include "mergesort.h"

int main(){
    int sz, key[]={4,3,1,67,55,8,0,4,-5,37,7,4,2,1,-1};
    sz=sizeof(key)/sizeof(int);
    printf("Before mergesort:\n");
    print_arr(key,sz);
    merge_sort(key,sz);
    printf("After mergesort:\n");
    print_arr(key,sz);
    return EXIT_SUCCESS;
}