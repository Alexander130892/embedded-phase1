/*
 * File:    Ex_06_forktwice_execl.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This file creates two child processes using fork(), where one
 *   child executes the "pgm2" program with arguments "1", "2", "3",
 *   "go" and the other executes the "pgm3" program with no arguments.
 *   The program uses execl() to replace both the child and parent
 *   processes with different executable programs, creating concurrent
 *   execution of two separate programs.
 */

// Compute Fibonacci numbers and print time asynchronously

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
int fork(void);

int main(void)
{
    if(fork() == 0){
        execl("pgm2", "pgm2", "1", "2", "3", "go", 0);
    }
    else{
        execl("pgm3", "pgm3", 0);
    }
    return EXIT_SUCCESS;
}
