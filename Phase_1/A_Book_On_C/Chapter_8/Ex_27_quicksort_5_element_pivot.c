/*
 * File:    Ex_27_quicksort_5_element_pivot.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This file implements a quicksort algorithm that uses a 5-element
 *   median-of-five pivot selection strategy, sampling five
 *   evenly-spaced elements from the array and selecting their median
 *   as the pivot to improve performance. The implementation appears to
 *   sort in descending order based on the modified comparison
 *   operators in the swap and partition logic.
 */
//write pack into one line

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

#define N 1000000
#define swap(x,y) {int t; t=x; x=y;y=t;}
#define order(x,y) if(x<y) swap(x,y) //changed
#define o2(x,y)             order(x,y)
#define o3(x,y,z)           o2(x,y);o2(x,z);o2(y,z);
#define o4(a,b,c,d)         o3(a,b,c);o2(a,d);o2(b,d);o2(c,d);
#define o5(a,b,c,d,e)       o4(a,b,c,d);o2(a,e);o2(b,e);o2(c,e);o2(d,e);

typedef enum{yes, no} yes_no;

static yes_no find_pivot(int* left, int* right, int * pivot_ptr);
static int* partition(int* left, int* right, int pivot);
void quicksort(int *, int *);
void print_arr(int* arr, int size);

int main(void){ 
    int i, a[N];
    srand(time(NULL));
    for(i=0; i<N;i++)
        a[i]= rand() % 1000;
    print_arr(a, N);
    quicksort(a, a+N-1);
    printf("----- AFTER SORTING ------\n ");
    quicksort(a, a+N-1);
    quicksort(a, a+N-1);
    quicksort(a, a+N-1);
    print_arr(a, N);
    return 0;
}
void print_arr(int* arr, int size)
{
    int i;
    for ( i = 0 ; i < 10; i++)
        printf("%d ", arr[i]);
    printf("......\n ");
    for ( i = size/2 ; i < size/2 +10 ; i++)
        printf("%d ", arr[i]);
    printf("......\n ");
    for ( i = size-10 ; i < size ; i++)
        printf("%d ", arr[i]);
    printf("\n ");
}
void quicksort(int * left, int * right){
    int *p, pivot;
    if(find_pivot(left, right, &pivot) == yes){
        p = partition(left, right, pivot);
        quicksort(left, p-1);
        quicksort(p, right);
    }
}

static yes_no find_pivot(int* left, int *right, int*pivot_ptr)
{
    int a, b, c, d, e, *p;
    ptrdiff_t gap = (right - left) / 4;   // divide into 4 equal parts → 5 points

    a = *left;
    b = *(left + gap);
    c = *(left + 2*gap);
    d = *(left + 3*gap);
    e = *right;

    o5(a, b, c, d, e);    // sort all five

    if(a>b){  // median is c — the middle value
        *pivot_ptr=c;
        return yes;
    }if(b>c){ // changed
        *pivot_ptr =c ;
        return yes;
    }
    for (p=left+1; p<=right; ++p)
    {
        if(*p != *left){
            *pivot_ptr=(*p > *left) ? *left : *p;
            return yes;
        }
    }
    return no;
}

static int* partition(int* left, int* right, int pivot)
{
    while( left <= right)
    {
        while (*left > pivot) //changed
        {
            left++;
        }
        while (*right <= pivot) //changed
        {
            right--;
        }
        if(left < right)
        {
            swap(*left, *right);
            left++;
            right--;
        }
    }
    return left;
}