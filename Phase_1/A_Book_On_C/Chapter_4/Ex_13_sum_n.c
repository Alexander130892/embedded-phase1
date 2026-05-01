/*
 * File:    Ex_13_sum_n.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file calculates the sum of integers from abs(n) to 2*abs(n)-1
 *   using both for and while loops, then adjusts the sign based on
 *   whether the input n is positive or negative. It contains a bug in
 *   the printf statement that calls putchar('1') instead of printing
 *   sum_for, and has unused constants and headers that suggest it may
 *   be part of a larger exercise.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define NCALLS 100000000 // number of fct calls
#define NCOLS 8 // number of collumns
#define NLINES 3 // number of lines


//Typedefs

//Prototype Functions

//MAIN
int main(){
    int n;
    printf("Give a value for n: ");
    scanf("%d", &n);
    int i;
    int sum_for=0;
    int sum_while=0;
     
    for (i=abs(n);i<2*abs(n);i++){
        sum_for+=i;
    }
    i=abs(n);
    while(i<2*abs(n)){
        sum_while+=i;
        i++;
    }
    sum_for*=n/abs(n);
    sum_while*=n/abs(n);
    printf("%d %d\n", putchar('1'), sum_while);
}

//Helper-Functions