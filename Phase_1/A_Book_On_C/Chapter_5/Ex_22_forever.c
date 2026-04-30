/*
 * File:    Ex_22_forever.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program creates an infinite loop that repeatedly calls
 *   function `f()`, which prints an incrementing counter and
 *   terminates the program when the counter reaches 17.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define STOP 17
#define FOREVER 1

//Typedefs

//Prototypes

int main(){
    void f(void);
    while(FOREVER)
        f();
    return 0;
}

void f(void){
    static int cnt = 0;
    printf("cnt = %d\n", ++cnt);
    if(cnt==STOP)
        exit(0);
}
