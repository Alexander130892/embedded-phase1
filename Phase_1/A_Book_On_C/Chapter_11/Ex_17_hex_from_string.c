/*
 * File:    Ex_17_hex_from_string.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates how to convert a hexadecimal string
 *   representation into an integer using sscanf, parsing "0xabc" and
 *   displaying its decimal equivalent value.
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(void)
{
    char s[] = "0xabc";
    int n;

    sscanf(s, "%x", &n);
    printf("Value of n: %d\n", n);
    return 0;
}
