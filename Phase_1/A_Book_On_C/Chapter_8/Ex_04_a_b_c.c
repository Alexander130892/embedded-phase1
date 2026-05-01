/*
 * File:    Ex_04_a_b_c.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C source file conditionally includes stdio.h and expands a
 *   macro A_B_C when the TRUE preprocessor condition is met, with the
 *   purpose appearing to be packing or consolidating code into a
 *   single line as indicated by the comment.
 */
//write pack into one line

#include  "Ex_04_a_b_c.h"

#if TRUE
    #include <stdio.h>
   
    A_B_C
#endif
