/*
 * File:    pgm3.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program simply executes the system "date" command to
 *   display the current date and time, then exits successfully.
 *   Despite including headers for time functions and defining
 *   constants, the program only uses the system() function to call the
 *   external date command.
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