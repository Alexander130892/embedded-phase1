/*
 * File:    Ex_04_fortune.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file creates a child process using fork() and executes the
 *   fortune program to display a random fortune/quote, while the
 *   parent process immediately exits.
 */
// Compute Fibonacci numbers and print time asynchronously

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

#define  N 13
int fork(void);

int32_t main(void)
{
    if(fork() == 0){
        execl("/opt/homebrew/bin/fortune", "fortune", 0);
    }
    return EXIT_SUCCESS;
}
