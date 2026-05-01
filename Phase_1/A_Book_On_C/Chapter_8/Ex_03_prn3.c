/*
 * File:    Ex_03_prn3.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program defines a macro PRN3 that prints the names and
 *   values of three float variables in a formatted line, then
 *   demonstrates it by displaying the values of variables x, y, and z
 *   along with their variable names using the stringification operator
 *   (#).
 */
//write pack into one line
#include <stdio.h>

#define PRN3(x,y,z) printf(#x " has value %.1f and " #y " has value %.1f and " #z " has value %.1f\n", x,y,z)

int main(void){ 
    float x=1.1, y=2.2, z=3.3;
    PRN3(x,y,z);
    return 0;
}
