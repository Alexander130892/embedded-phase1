/*
 * File:    Ex_10_try_me copy.c
 * Author:  Alexander130892
 * Date:    11-4-2026
 *
 * Description:
 *   This program demonstrates the difference between standard output
 *   and standard error streams by printing the first part of a tongue
 *   twister to stdout and the second part to stderr.
 */
#include <stdio.h>

int main(void)
{
    fprintf(stdout, "She sells sea shells\n");
    fprintf(stderr, "by the seashore.\n");
    return 0;
}