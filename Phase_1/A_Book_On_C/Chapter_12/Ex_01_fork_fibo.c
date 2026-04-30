/*
 * File:    Ex_01_fork_fibo.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This program uses fork() to create two processes that run
 *   concurrently: a child process that computes and prints the first
 *   30 Fibonacci numbers, and a parent process that prints elapsed
 *   time every second. The processes execute asynchronously,
 *   demonstrating parallel computation where Fibonacci calculations
 *   run alongside time tracking.
 */

// Compute Fibonacci numbers and print time asynchronously

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

uint32_t    fib(uint8_t);
int32_t     fork(void);
void        sleep(uint32_t);

int32_t main(void)
{
    uint32_t    begin = time(NULL);
    uint8_t     i;

    if (fork() == 0)    // Child process
    {
        for(i = 0; i < 30; i++ )
        {
            printf("fib(%2d) = %d\n", i, fib(i));
        }
    }else       // Parent Process
    {
        for (i = 0; i < 30; i++)
        {
            sleep(1);
            printf("elapsed time = %ld\n", time(NULL) - begin);
        }
    }
    return EXIT_SUCCESS;
}

uint32_t fib(uint8_t n){
    if (n <= 1)
    {
        return 1;
    }else
    {
        return (fib(n-1) + fib(n-2));
    }
}