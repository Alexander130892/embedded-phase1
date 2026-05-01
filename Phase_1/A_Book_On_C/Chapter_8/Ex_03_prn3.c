/*
 * File:    Ex_03_prn3.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates a C macro that prints three float variables
 *   along with their names and values in a formatted string, using the
 *   stringification operator (#) to convert variable names to literal
 *   strings in the output.
 */
//write pack into one line
#include <stdio.h>

#define PRN3(x,y,z) printf(#x " has value %.1f and " #y " has value %.1f and " #z " has value %.1f\n", x,y,z)

int main(void){ 
    float x=1.1, y=2.2, z=3.3;
    PRN3(x,y,z);
    return 0;
}
