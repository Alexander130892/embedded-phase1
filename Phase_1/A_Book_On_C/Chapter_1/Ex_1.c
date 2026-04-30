/*
 * File:    Ex_1.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates three different ways to display the phrase
 *   "She sells sea shells by the seashore" using printf statements: as
 *   a single line, split across three lines, and formatted within a
 *   decorative asterisk border box.
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