/*
 * File:    Ex_19_union.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This file demonstrates C union declarations, showing both a simple
 *   union with multiple data types sharing memory space and a typedef
 *   union that uses bit fields to represent a status register, but
 *   contains no functional implementation in the main function.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
union a {
    int a;
    char b;
    float c; //needs to be different
} a, b, c;

typedef union {
    uint8_t raw;
    struct {
        uint8_t ready   : 1;
        uint8_t error   : 1;
        uint8_t mode    : 2;
        uint8_t padding : 4;
    } bits;
} StatusRegister;



int main(){
    //init resto lists
    
    return EXIT_SUCCESS;
}

