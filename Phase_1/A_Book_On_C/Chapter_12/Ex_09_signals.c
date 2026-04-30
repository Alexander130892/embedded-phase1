/*
 * File:    Ex_09_signals.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates signal handling in C by performing division
 *   by zero operations, first ignoring the SIGFPE (floating-point
 *   exception) signal and then restoring the default signal behavior.
 *   The program shows how different signal handling approaches affect
 *   the execution when encountering floating-point arithmetic errors.
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
