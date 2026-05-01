/*
 * File:    Ex_10_try_me copy.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program demonstrates the difference between stdout and
 *   stderr output streams by printing the first part of a tongue
 *   twister to stdout and the second part to stderr.
 */
#include <stdio.h>

int main(void)
{
    fprintf(stdout, "She sells sea shells\n");
    fprintf(stderr, "by the seashore.\n");
    return 0;
}