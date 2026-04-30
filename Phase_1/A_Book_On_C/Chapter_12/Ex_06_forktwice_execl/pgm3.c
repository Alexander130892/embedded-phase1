/*
 * File:    pgm3.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This file executes the system date command and then exits
 *   successfully, displaying the current date and time to the console.
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