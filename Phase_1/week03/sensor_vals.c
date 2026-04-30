/*
 * File:    sensor_vals.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This file demonstrates packing and unpacking of two 4-bit sensor
 *   values into a single 8-bit byte for efficient data storage or
 *   transmission. It includes utility functions to combine two sensor
 *   readings into one byte, extract them back to separate values, and
 *   display their binary representations.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//Consts

//Typedefs
typedef unsigned char sensValue; //assume sens value is 4 bit signal

//Prototype Functions
sensValue pack(sensValue a, sensValue b);
void unpack(sensValue byte, sensValue* a, sensValue* b);
void bitprint(unsigned char byte);
//MAIN //pack 2 sensor values into one byte char and unpack it again
int main(){
    sensValue a = 0x0A;
    sensValue b = 0x03;
    bitprint(a);
    bitprint(b);
    sensValue AB=0;
    AB=pack(a,b);
    bitprint(AB);
    sensValue c=0;
    sensValue d=0;
    unpack(AB, &c, &d);
    bitprint(c);
    bitprint(d);
    return EXIT_SUCCESS;
}

sensValue pack(sensValue a, sensValue b){
    return (a << 4) | b;
}
void unpack(sensValue byte, sensValue* a, sensValue* b){
    sensValue mask = 0xF; // 0000 1111
    *b = mask & byte;
    *a = (byte >> 4) & mask;
    return;
}
void bitprint(sensValue byte)
{
    int i;
    int n = sizeof(sensValue) *CHAR_BIT;
    sensValue mask = 1;
    mask <<= (n-1); //100..00
    for (i = 1 ; i <= n ; i++ )
    {
        putchar(((mask & byte)==0)?'0':'1');
        mask >>= 1;
    }
    putchar('\n');
}