/*
 * File:    Ex_06_predefMacro.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This file demonstrates the use of predefined C preprocessor macros
 *   by printing compilation information including the date, filename,
 *   line number, C standard compliance flag, and compilation time.
 */
//write pack into one line
#include <stdio.h>

#define PRN3(x,y,z) printf(#x " has value %.1f and " #y " has value %.1f and " #z " has value %.1f\n", x,y,z)

int main(void){ 
    printf("%s%s\n%s%s\n%s%d\n%s%d\n%s%s\n",
        "__DATE__ = ", __DATE__,
        "__FILE__ = ", __FILE__,
        "__LINE__ = ", __LINE__,
        "__STDC__ = ", __STDC__,
        "__TIME__ = ", __TIME__);
    return 0;
}
