/*
 * File:    Ex_13_sum_n.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This C program calculates the sum of integers from |n| to 2|n|-1
 *   using both for and while loops, then applies the sign of the
 *   original input n to both results, though it only prints the while
 *   loop result along with a character output.
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