/*
 * File:    Ex_02_fork_limit.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file creates a binary tree of processes by forking N times in
 *   a loop, with each parent and child process continuing to fork,
 *   resulting in exponential process creation where both parent and
 *   child processes print identifying messages at each iteration.
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
