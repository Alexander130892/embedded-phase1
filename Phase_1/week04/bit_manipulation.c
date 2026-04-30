/*
 * File:    bit_manipulation.c
 * Author:  Alexander130892
 * Date:    3-4-2026
 *
 * Description:
 *   This file implements and demonstrates basic bit manipulation
 *   operations through macros that can set, clear, toggle, and read
 *   individual bits in data values. It includes two different bit
 *   printing functions to visualize the binary representation of data
 *   and test the bit manipulation macros on an unsigned char value.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//Consts

//Macros
#define SETBIT(d, N) d = (d | ( (uintmax_t) 1u << (N-1))) 
#define CLEARBIT(d, N) d = (d & ~( (uintmax_t) 1u << (N-1))) 
#define TOGGLEBIT(d, N) d = (d ^ ( (uintmax_t) 1u << (N-1)))
#define READBIT(d, N) (d >> (N-1) & (uintmax_t) 1u)
//Typedefs
typedef unsigned char data;

//Prototype Functions
void bitprint(data input);
void bitprint_macro(data input);

//MAIN 
int main(){
    data d= 0xA8;
    bitprint(d);
    SETBIT(d,7);
    bitprint(d);
    CLEARBIT(d,8);
    bitprint(d);
    CLEARBIT(d,1);
    bitprint(d);
    TOGGLEBIT(d,1);
    bitprint(d);
    data b = READBIT(d, 2);
    printf("%d\n", b);
    bitprint_macro(d);
    return EXIT_SUCCESS;
}

void bitprint(data input){ // for testing, without macros
    int n = sizeof(data) * CHAR_BIT;
    data mask = 1; //00..01
    mask <<= (n-1); //10...0
    for(int i = 1 ; i <=n ; i++){
        putchar((mask & input)?'1':'0');
        mask >>= 1;        
        if(i % CHAR_BIT ==0){
            putchar(' ');
        }
    }
    putchar('\n');
}

void bitprint_macro(data input){
    int n = sizeof(data) * CHAR_BIT;
    data b;
    for(int i = n ; i >=1 ; i--){
        b = READBIT(input, i);
        printf("%d", b);    
        if((i-1) % CHAR_BIT == 0){
            putchar(' ');
        }
    }
    putchar('\n');
}
