/*
 * File:    Ex_2.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program prints a string containing null terminators and
 *   demonstrates that printf() only outputs characters up to the first
 *   null terminator, then displays the count of characters actually
 *   printed (which will be 3 for "abc").
 */
#include <stdio.h>
#include <stdlib.h>

int main(){

   int cnt;
   cnt=printf("abc\0abc\0");
   printf("Number of successful conversions: %d\n", cnt);
   
}