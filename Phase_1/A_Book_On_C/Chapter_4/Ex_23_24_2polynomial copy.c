/*
 * File:    Ex_23_24_2polynomial copy.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file solves quadratic equations of the form ax²+bx+c=0 by
 *   calculating and displaying the real or complex roots using the
 *   quadratic formula. It handles special cases including linear
 *   equations (when a=0) and displays complex roots when the
 *   discriminant is negative.
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
    int a,b,c,i;
    double D,x1,x2;
    printf("give values for a, b,c in ax^2+bx+c=0: ");
    scanf("%d %d %d", &a,&b,&c);
    if (!a && !b)
        printf("No solution possible\n");
    else if(!a)
        printf("single root x1 = %.2lf\n", -(double)c/b);
    else{
        D=b*b-4*a*c;
        if (D<0){
            double x1_i;
            double x2_i;
            x1=(-b)/(2*a);
            x1_i=(sqrt(-D))/(2*a);
            i=1;
            printf("The roots of %dx^2+%dx+%d=0 are: x1 = %.2lf + i*%.2lf and x2 = %.2lf - i*%.2lf\n", a,b,c, x1, x1_i, x1, x1_i);
        }else{
            x1=(-b+sqrt(D))/(2*a);
            x2=(-b-sqrt(D))/(2*a);
            i=0;
            printf("The roots of %dx^2+%dx+%d=0 are: x1 = %s%.2lf and x2 = %s%.2lf \n", a,b,c, i?"i":"",x1, i?"i":"",x2);
    }
    
    }
}

//Helper-Functions