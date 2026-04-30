/*
 * File:    Ex_25_quicksort_small_array.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This file implements a hybrid quicksort algorithm that switches to
 *   insertion sort for small subarrays (fewer than 7 elements) to
 *   optimize performance on small datasets. It sorts an array of
 *   10,000 random integers between 0-9 and uses a sophisticated pivot
 *   selection strategy that chooses the median of three values or
 *   finds a suitable pivot by linear search.
 */
//write pack into one line

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000
#define swap(x,y) {int t; t=x; x=y;y=t;}
#define order(x,y) if(x>y) swap(x,y)
#define o2(x,y) order(x,y)
#define o3(x,y,z) o2(x,y);o2(x,z);o2(y,z);

typedef enum{yes, no} yes_no;

static yes_no find_pivot(int* left, int* right, int * pivot_ptr);
static int* partition(int* left, int* right, int pivot);
int quicksort(int *, int *);
void print_arr(int* arr, int size);

int main(void){ 
    int i, a[N];
    int c;
    srand(time(NULL));
    for(i=0; i<N;i++)
        a[i]= rand() % 10;
    print_arr(a, N);
    c=quicksort(a, a+N-1);
    printf("----- AFTER SORTING ------%d\n ",c);
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
int quicksort(int * left, int * right){
    int *p,*q, pivot;
    static int cnt =0;
    if(right - left < 7){
        for(p=left ; p < right ; ++p){
            for (q = p+1; q <= right;++q){
                if(*p > *q)
                    swap(*p, *q);
            }
        }
    }else if(find_pivot(left, right, &pivot) == yes){
            p = partition(left, right, pivot);
            quicksort(left, p-1);
            quicksort(p, right);
        }
    return ++cnt;
}

static yes_no find_pivot(int* left, int *right, int*pivot_ptr)
{
    int a,b,c,*p;
    a= *left;
    b= *(left + (right-left)/2); // middle value
    c= *right;
    o3(a,b,c);  //order these 3 values
    if(a<b){ //pivot will be the highest of these 2
        *pivot_ptr=b;
        return yes;
    }if(b<c){
        *pivot_ptr =c ;
        return yes;
    }
    for (p=left+1; p<=right; ++p)
    {
        if(*p != *left){
            *pivot_ptr=(*p < *left) ? *left : *p;
            return yes;
        }
    }
    return no;
}

static int* partition(int* left, int* right, int pivot)
{
    while( left <= right)
    {
        while (*left < pivot)
        {
            left++;
        }
        while (*right >= pivot)
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