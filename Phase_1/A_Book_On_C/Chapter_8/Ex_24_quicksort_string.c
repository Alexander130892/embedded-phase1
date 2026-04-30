/*
 * File:    Ex_24_quicksort_string.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This C program implements a quicksort algorithm specifically for
 *   sorting arrays of strings using lexicographic comparison via
 *   strcmp(). The program demonstrates sorting a hardcoded array of 5
 *   strings and includes a sophisticated pivot selection strategy that
 *   uses the median-of-three method with additional logic to handle
 *   duplicate values.
 */
//write pack into one line

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 5
typedef char* str;
#define swap(x,y) {str t; t=x; x=y;y=t;}
#define order(x,y) if(strcmp(x,y)>0) swap(x,y)
#define o2(x,y) order(x,y)
#define o3(x,y,z) o2(x,y);o2(x,z);o2(y,z);

typedef enum{yes, no} yes_no;

static yes_no find_pivot(str* left, str* right, str * pivot_ptr);
static str* partition(str* left, str* right, str pivot);
void quicksort(str *, str *);
void print_arr(str* arr, int size);

int main(void){ 
    int i;
    str a[5]={"aaaa", "ccc","zzz","dea","bbbbb"};
    print_arr(a, N);
    quicksort(a, a+N-1);
    printf("----- AFTER SORTING ------\n ");
    print_arr(a, N);
    return 0;
}
void print_arr(str* arr, int size)
{
    int i;
    for ( i = 0 ; i < size; i++)
        printf("%s ", arr[i]);
    printf("\n ");
}
void quicksort(str * left, str * right){
    str *p, pivot;
    if(find_pivot(left, right, &pivot) == yes){
        p = partition(left, right, pivot);
        quicksort(left, p-1);
        quicksort(p, right);
    }
}

static yes_no find_pivot(str* left, str *right, str*pivot_ptr)
{
    str a,b,c,*p;
    a= *left;
    b= *(left + (right-left)/2); // middle value
    c= *right;
    o3(a,b,c);  //order these 3 values
    if(strcmp(a,b) < 0){
        *pivot_ptr = b;
        return yes;
    }
    if(strcmp(b,c) < 0){
        *pivot_ptr = c;
        return yes;
    }
    for(p = left+1; p <= right; ++p){
        if(strcmp(*p, *left) != 0){
            *pivot_ptr = (strcmp(*p, *left) < 0) ? *left : *p;
            return yes;
        }
    }
    return no;
}

static str* partition(str* left, str* right, str pivot)
{
    while( left <= right)
    {
        while(strcmp(*left,  pivot) < 0)  left++;
        while(strcmp(*right, pivot) >= 0) right--;
        if(left < right)
        {
            swap(*left, *right);
            left++;
            right--;
        }
    }
    return left;
}