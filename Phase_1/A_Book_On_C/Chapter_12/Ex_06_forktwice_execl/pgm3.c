/*
 * File:    pgm3.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program simply displays the current system date and time by
 *   executing the "date" command through a system call, then exits
 *   successfully.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>


#define  N 13
int fork(void);

int32_t main(int argc,char** argv)
{
    system("date");
    return EXIT_SUCCESS;
}