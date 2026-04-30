/*
 * File:    Ex_04_a_b_c.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This C source file conditionally includes stdio.h and expands a
 *   macro A_B_C when the TRUE preprocessor condition is met, with a
 *   comment suggesting code should be packed into one line.
 */
//write pack into one line

#include  "Ex_04_a_b_c.h"

#if TRUE
    #include <stdio.h>
   
    A_B_C
#endif
