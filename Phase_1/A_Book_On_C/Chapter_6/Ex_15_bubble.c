/*
 * File:    Ex_15_bubble.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a bubble sort algorithm to sort an array of
 *   double values in ascending order, with an optimized version that
 *   stops early if no swaps are made during a pass. The program
 *   demonstrates the sorting by printing an array before and after
 *   applying the bubble sort function.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>

//Consts
#define N 5

//Typedefs

//Prototype Functions
void add_p(double *f, double *g, double *h, int K);
void mul_p(double *f, double *g, double *h, int K);
void print_p(double *p, int degree);
void bubble(double *, int);

//MAIN
int main(){
    double g[N+1]   = {1, 2, 3, 2, -1, 0};  
    double h[N+1]   = {1, 1, 4, 5, 4, 1};  
    print_p(g, N+1);   
    bubble(g, N+1);
    print_p(g, N+1);                        

    return EXIT_SUCCESS;
}

//Helper-Functions
void bubble(double * arr, int size){
    double temp;
    int switched;
    for(int i=0;i<size-1;i++){
        switched=0;
        for(int j=0;j<size-1-i;j++){
            if(arr[j]>arr[j+1]){
                temp = arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
                switched=1;
            }
        }
        if (switched==0)
            break;
    }
}

void print_p(double *p, int degree){
    for(int i = 0; i < degree; i++)
        printf("%.0f, ", p[i]);
    putchar('\n');
}