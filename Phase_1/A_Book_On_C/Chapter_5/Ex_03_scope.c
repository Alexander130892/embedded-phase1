/*
 * File:    Ex_03_scope.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates variable scope in C by showing how a global
 *   variable `z` is modified by a function while a local parameter `x`
 *   remains unchanged, illustrating the difference between
 *   pass-by-value and global variable access.
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

