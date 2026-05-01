/*
 * File:    Ex_21_BCD.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates Binary Coded Decimal (BCD) encoding by
 *   converting an unsigned integer to a BCD structure where each
 *   decimal digit is stored in a separate 4-bit field, then converting
 *   it back to verify the process. The program uses bit fields to
 *   efficiently pack 8 decimal digits into a single structure and
 *   includes functions to convert between binary and BCD
 *   representations.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
typedef struct{
    unsigned int dig_1 :4;
    unsigned int dig_2 :4;
    unsigned int dig_3 :4;
    unsigned int dig_4 :4;
    unsigned int dig_5 :4;
    unsigned int dig_6 :4;
    unsigned int dig_7 :4;
    unsigned int dig_8 :4;
} BCD;

BCD binary_to_bcd(unsigned int input);
void print_bcd(BCD input);
unsigned int bcd_to_binary(BCD input);


int main(){
    unsigned int a = 12345678;
    printf("%x\n", a);
    BCD b;
    b=binary_to_bcd(a);
    print_bcd(b);
    printf("%x\n", (bcd_to_binary(b)));
    return EXIT_SUCCESS;
}

BCD binary_to_bcd(unsigned int input)
{
    BCD output;
    output.dig_8 = input % 10; input /= 10;
    output.dig_7 = input % 10; input /= 10;
    output.dig_6 = input % 10; input /= 10;
    output.dig_5 = input % 10; input /= 10;
    output.dig_4 = input % 10; input /= 10;
    output.dig_3 = input % 10; input /= 10;
    output.dig_2 = input % 10; input /= 10;
    output.dig_1 = input % 10; input /= 10;
    return output;
}
void print_bcd(BCD input)
{
    printf("%d ", input.dig_1);
    printf("%d ", input.dig_2);
    printf("%d ", input.dig_3);
    printf("%d ", input.dig_4);
    printf("%d ", input.dig_5);
    printf("%d ", input.dig_6);
    printf("%d ", input.dig_7);
    printf("%d ", input.dig_8);
    putchar('\n');
    return;
}
unsigned int bcd_to_binary(BCD input){
    unsigned int output = 0;
    output += input.dig_1;
    output *= 10;
    output += input.dig_2;
    output *= 10;
    output += input.dig_3;
    output *= 10;
    output += input.dig_4;
    output *= 10;
    output += input.dig_5;
    output *= 10;
    output += input.dig_6;
    output *= 10;
    output += input.dig_7;
    output *= 10;
    output += input.dig_8;
    return output;
}