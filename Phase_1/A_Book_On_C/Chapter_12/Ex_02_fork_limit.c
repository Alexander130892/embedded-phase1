/*
 * File:    Ex_02_fork_limit.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This program creates a binary tree of processes by forking 13
 *   times in a loop, with each parent and child process printing
 *   identification messages before potentially forking again. The
 *   program demonstrates exponential process creation where each
 *   successful fork doubles the number of running processes,
 *   potentially creating thousands of processes that all execute the
 *   remaining loop iterations.
 */

// Compute Fibonacci numbers and print time asynchronously

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define  N 13
int fork(void);

int32_t main(void)
{
    for(int i=1;i<=N; i++){
        int pid = fork();
        if(pid == 0){
            printf("%2d: Hello from child\n", i);
        }else if (pid > 0){
            printf("%2d: Hello from parent\n", i);
        }else{
            printf("%2d: ERROR - Fork did not occur ERRORERRORERRORERRORERRORERROR\n", i);
        }
    }
    return EXIT_SUCCESS;
}
