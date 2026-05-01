/*
 * File:    Ex_02_fork_limit.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This program creates a binary tree of processes by calling fork()
 *   13 times in a loop, with each successful fork creating both a
 *   parent and child process that continue executing the loop,
 *   resulting in exponential process creation (up to 2^13 processes)
 *   until system limits are reached and fork() begins to fail.
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
