/*
 * File:    Ex_03_prn3.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This file demonstrates a C macro that prints the names and values
 *   of three float variables in a formatted string, using the
 *   stringification operator (#) to convert variable names into string
 *   literals for display.
 */
//write pack into one line
#include <stdio.h>

#define PRN3(x,y,z) printf(#x " has value %.1f and " #y " has value %.1f and " #z " has value %.1f\n", x,y,z)

int main(void){ 
    float x=1.1, y=2.2, z=3.3;
    PRN3(x,y,z);
    return 0;
}
