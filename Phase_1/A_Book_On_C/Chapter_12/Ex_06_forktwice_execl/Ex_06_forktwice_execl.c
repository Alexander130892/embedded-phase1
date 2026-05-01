/*
 * File:    Ex_06_forktwice_execl.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file creates two child processes using fork() and replaces
 *   each with different programs using execl() - one child executes
 *   "pgm2" with arguments "1", "2", "3", "go", while the parent
 *   process executes "pgm3" with no arguments.
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
