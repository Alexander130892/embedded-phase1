/*
 * File:    Ex_28_set_union.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program implements set operations using bitwise operations
 *   on unsigned long integers, where each bit represents a set
 *   element, and demonstrates union, intersection, and complement
 *   operations with both binary and element-based display functions.
 */
//write pack into one line
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long set;
const set empty =0x00;

set Union(set a, set b);
set intersection(set a, set b);
set complement(set a);
void display(set a);
void display_set(set a);

int main(void){ 
    set a = 0x7; // 0000 0111
    set b = 0x55; // 0101 0101
    display(a);
    display_set(a);
    display(b);
    display_set(b);
    display(Union(a,b));
    display_set(Union(a,b));
    display(intersection(a,b));
    display_set(intersection(a,b));
    display(complement(a));
    display_set(complement(a));
    return EXIT_SUCCESS;
}

set Union(set a, set b){
    return a | b;
}
set complement(set a){
    return ~a;
}
set intersection(set a, set b){
    return a & b;
}

void display(set a){
    int i;
    int n = sizeof(set) * CHAR_BIT;
    set mask = (set) 1 << (n-1); // 100..00
    for(i=1; i<=n; i++){
        putchar(((a & mask)==0)?'0':'1');
        a <<= 1;
        if( i % CHAR_BIT == 0 && i<n)
            putchar(' ');
    }
    putchar('\n');
}

void display_set(set a){
    int i;
    int n = sizeof(set) * CHAR_BIT;
    set mask = (set) 1; // 000..01
    printf("The set contains: ");
    for(i=1; i<=n; i++){
        if(mask & a)
            printf("%d, ", i);
        mask <<= (set) 1;
    }
    putchar('\n');
}
