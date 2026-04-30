/*
 * File:    Ex_2.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program demonstrates how printf handles null terminators by
 *   printing a string containing embedded null characters and
 *   displaying the count of characters actually printed. The printf
 *   function stops at the first null terminator, so it prints only
 *   "abc" and returns 3, even though the string literal contains more
 *   characters after the null.
 */

#include <stdio.h>
#include <stdlib.h>

int main(){

   int cnt;
   cnt=printf("abc\0abc\0");
   printf("Number of successful conversions: %d\n", cnt);
   
}