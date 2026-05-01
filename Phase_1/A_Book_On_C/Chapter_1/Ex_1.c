/*
 * File:    Ex_1.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates different ways to format and display the
 *   phrase "She sells sea shells by the seashore" using printf
 *   statements - first as a single line, then split across three
 *   lines, and finally enclosed in a decorative box made of asterisks.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void){
    //oneline
    printf("She sells sea shellss by the seashore\n");
    //3 lines
    printf("She sells \nsea shells \nby the seashore\n");
    // in a box
    printf("**************************************************\n");
    printf("*\tShe sells sea shells by the seashore\t*\n");
    printf("**************************************************\n");
   return EXIT_SUCCESS;
}