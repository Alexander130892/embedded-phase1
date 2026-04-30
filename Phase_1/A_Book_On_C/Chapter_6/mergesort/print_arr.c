/*
 * File:    print_arr.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file implements a utility function that prints an integer
 *   array in a formatted bracketed list, displaying each element with
 *   3-character width spacing and separating elements with spaces
 *   before closing with a bracket and newline.
 */
#include "mergesort.h"

void print_arr(int* arr, int size){
    putchar('[');
    for (int i=0;i<size;i++){
        printf("%3d%s", arr[i],((i<size-1)?" ":"]\n"));
    }
}