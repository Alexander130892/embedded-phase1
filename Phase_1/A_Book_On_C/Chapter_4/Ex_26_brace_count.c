/*
 * File:    Ex_26_brace_count.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program reads input character by character, counts matching
 *   pairs of curly braces `{}` and parentheses `()`, and outputs the
 *   original text while marking syntax errors with `?` when closing
 *   braces appear before their corresponding opening braces. After
 *   processing all input, it reports whether the braces are properly
 *   balanced and suggests missing closing braces if needed.
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