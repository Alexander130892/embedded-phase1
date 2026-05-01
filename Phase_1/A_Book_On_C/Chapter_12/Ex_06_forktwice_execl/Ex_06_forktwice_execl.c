/*
 * File:    Ex_06_forktwice_execl.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program creates a child process using fork() and then
 *   replaces both the parent and child processes with different
 *   executables using execl() - the child runs "pgm2" with arguments
 *   "1", "2", "3", "go", while the parent runs "pgm3" with no
 *   arguments.
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
