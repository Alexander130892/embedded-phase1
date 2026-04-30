/*
 * File:    Ex_01_fork.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This program demonstrates the fork() system call by creating three
 *   successive child processes, resulting in 8 total processes (2^3)
 *   that each print their respective fork() return value. The output
 *   will show multiple "In main value = X" messages where X is 0 for
 *   child processes and the child's process ID for parent processes.
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