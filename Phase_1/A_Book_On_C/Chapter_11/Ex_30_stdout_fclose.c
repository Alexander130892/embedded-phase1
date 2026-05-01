/*
 * File:    Ex_30_stdout_fclose.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This program demonstrates what happens when stdout is closed
 *   mid-execution by printing "Hello!", closing the stdout stream with
 *   fclose(), then attempting to print "Goodbye!" which will fail
 *   silently since stdout is no longer available.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>

#define MAXSTRING 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING 100

int main(int argc, char **argv)
{
    printf("Hello!\n");
    fclose(stdout);
    printf("Goodbye!\n");
    return EXIT_SUCCESS;
}