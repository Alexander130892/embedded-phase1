/*
 * File:    Ex_09_signals.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This C program demonstrates signal handling for floating-point
 *   exceptions (SIGFPE) by first ignoring division by zero errors and
 *   printing the result, then restoring default signal handling which
 *   would typically cause the program to terminate on the second
 *   division by zero.
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
