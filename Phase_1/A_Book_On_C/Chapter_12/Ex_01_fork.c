/*
 * File:    Ex_01_fork.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates the fork() system call by creating multiple
 *   child processes through three consecutive fork() calls, resulting
 *   in 8 total processes that each print the return value of the last
 *   fork() call (which will be 0 for child processes and the child's
 *   PID for parent processes).
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int fork(void);
    int value;

    value = fork(); // New Process
    value = fork(); // New Process
    value = fork(); // New Processs
    printf("In main value = %d\n", value);
    return EXIT_SUCCESS;
}