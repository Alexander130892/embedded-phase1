/*
 * File:    Ex_26_brace_count.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file reads input character by character and counts matching
 *   pairs of braces `{}` and parentheses `()`, echoing the input while
 *   marking imbalanced braces with a `?` character and providing error
 *   messages indicating missing opening or closing braces. It serves
 *   as a basic syntax checker for brace and parenthesis matching in
 *   text input.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define NCALLS 100000000 // number of fct calls
#define NCOLS 8 // number of collumns
#define NLINES 3 // number of lines


//Typedefs

//Prototype Functions

//MAIN
int main(){
    int left=0,right =0;
    int left_p=0,right_p=0;
    int c;
    while((c=getchar())!=EOF){
        switch(c){
            case '{':
                left++;
                break;
            case '}':
                right++;
                break;
            case '(':
                left_p++;
                break;
            case ')':
                right_p++;
                break;
        }
        putchar(c);
        if(right_p>left_p || right > left){
            putchar('?');
            //putchar('?');
        }
    }
    if(right == left && right_p == left_p)
        printf("No error!\n");
    else if(left > right || left_p > right_p){
        printf("ERROR: Missing right braces: %d %d ", left_p, right_p);
        for(int i=0;i<(left-right);i++)
            putchar('}');
        for(int i=0;i<(left_p-right_p);i++)
            putchar(')');
        putchar('\n');
    } else
        printf("ERROR: Missing left braces!");
}

//Helper-Functions