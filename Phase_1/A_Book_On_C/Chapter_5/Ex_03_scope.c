/*
 * File:    Ex_03_scope.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program demonstrates variable scope by showing how a global
 *   variable `z` is modified within a function while the local
 *   parameter `x` remains unchanged, ultimately printing the value 7
 *   after `z` is incremented by 2 inside function `f`.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
int z;


//Typedefs
void f(int x){
    x=2;
    z+=x;
}


int main(){
    z=5;
    f(z);
    printf("f(z)=%d",z);
    return 0;
}

