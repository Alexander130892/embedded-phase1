/*
 * File:    Ex_19_union.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C file demonstrates union data structures by defining a basic
 *   union with multiple data types sharing the same memory location
 *   and a more complex union that interprets a byte as either raw data
 *   or individual bit fields for status register manipulation, though
 *   the main function contains no actual implementation.
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

