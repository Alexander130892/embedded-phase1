/*
 * File:    pgm3.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This program simply displays the current date and time by
 *   executing the system "date" command and then exits successfully.
 *   The file contains unused includes and definitions (including a
 *   fork() declaration) that suggest it may be incomplete or part of a
 *   larger intended program.
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