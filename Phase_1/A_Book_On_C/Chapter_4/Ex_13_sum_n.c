/*
 * File:    Ex_13_sum_n.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program calculates the sum of integers from abs(n) to
 *   2*abs(n)-1 using both for and while loops, then multiplies the
 *   result by the sign of n to handle negative inputs. The program has
 *   a bug where it prints the return value of putchar('1') instead of
 *   sum_for, and contains unused constants that suggest it may be part
 *   of a larger exercise set.
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