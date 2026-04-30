/*
 * File:    Ex_30_stdout_fclose.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program demonstrates what happens when you close the stdout
 *   stream - it prints "Hello!" successfully, then closes stdout with
 *   fclose(), causing the subsequent "Goodbye!" printf to fail
 *   silently since there's no longer a valid output stream.
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