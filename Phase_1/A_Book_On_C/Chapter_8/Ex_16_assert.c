/*
 * File:    Ex_16_assert.c
 * Author:  Alexander130892
 * Date:    28-3-2026
 *
 * Description:
 *   This file demonstrates undefined behavior by left-shifting an
 *   integer by 65 bits (which exceeds the bit width of most integer
 *   types) and then uses an assertion to verify the resulting value is
 *   greater than 200000, likely causing unpredictable program behavior
 *   due to the invalid shift operation.
 */
//write pack into one line

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#define int long



int main(void){ 
    int a=1<<65, b=200000;
    printf("%d",a);
    assert(a > b);
    return 0;
}
