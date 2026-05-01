/*
 * File:    Ex_2.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates how printf handles null terminators by
 *   printing a string containing embedded null characters and then
 *   displays the count of characters actually printed, showing that
 *   printf stops at the first null terminator. The program prints
 *   "abc" followed by the count of 3 characters printed, illustrating
 *   that the second "abc" after the null terminator is ignored.
 */
#include <stdio.h>
#include <stdlib.h>

int main(){

   int cnt;
   cnt=printf("abc\0abc\0");
   printf("Number of successful conversions: %d\n", cnt);
   
}