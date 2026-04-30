/*
 * File:    Ex_22_flowcontrol.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates basic flow control in C by using a for loop
 *   to iterate through values 1-3 and a switch statement to execute
 *   different printf statements based on each iteration value.
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
    for (int i=1; i<=3;i++){
        switch(i){
            case 1:
                printf("1 - Statement 1 Executed\n");
                break;
            case 2:
                printf("1 - Statement 2 Executed\n");
                break;
            case 3:
                printf("1 - Statement 3 Executed\n");
                break;
            default:
                printf("1 - This Should not have been Executed\n");
        }
    }
}

//Helper-Functions