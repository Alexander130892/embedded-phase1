/*
 * File:    print_arr.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements a utility function that prints an integer
 *   array in a formatted style, displaying elements with 3-character
 *   spacing inside square brackets. The function uses a conditional
 *   operator to add spaces between elements and close the bracket with
 *   a newline after the last element.
 */
#include "mergesort.h"

void print_arr(int* arr, int size){
    putchar('[');
    for (int i=0;i<size;i++){
        printf("%3d%s", arr[i],((i<size-1)?" ":"]\n"));
    }
}