/*
 * File:    pgm3.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program simply executes the system "date" command to
 *   display the current date and time, then exits successfully.
 *   Despite including several headers and defining constants that
 *   suggest more complex functionality, the actual implementation only
 *   calls the system date command.
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