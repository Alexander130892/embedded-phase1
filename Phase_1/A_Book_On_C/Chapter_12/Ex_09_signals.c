/*
 * File:    Ex_09_signals.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates signal handling in C by first ignoring
 *   floating-point exceptions (SIGFPE) to safely perform division by
 *   zero, then restoring default signal handling which would typically
 *   terminate the program on the same operation.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

int main(void)
{
    double x = 3, y = HUGE_VAL;

    signal(SIGFPE, SIG_IGN);
    printf("Ignore signal: x / 0 = %e\n", x/0.0);
    signal(SIGFPE, SIG_DFL);
    printf("Default signal: x / 0 = %e\n", x/0.0);
    return 0;
}
