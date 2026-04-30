/*
 * File:    print_arr.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a utility function that prints an integer
 *   array in a formatted style with square brackets, displaying each
 *   element with 3-character width spacing. The function outputs
 *   elements separated by spaces and closes with a closing bracket and
 *   newline.
 */

#include "mergesort.h"

void print_arr(int* arr, int size){
    putchar('[');
    for (int i=0;i<size;i++){
        printf("%3d%s", arr[i],((i<size-1)?" ":"]\n"));
    }
}