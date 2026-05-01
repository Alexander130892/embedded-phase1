/*
 * File:    Ex_01_fork.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program demonstrates the fork() system call by creating three
 *   successive child processes, resulting in 8 total processes (1
 *   original + 7 children) that each print the return value of the
 *   last fork() call they executed.
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