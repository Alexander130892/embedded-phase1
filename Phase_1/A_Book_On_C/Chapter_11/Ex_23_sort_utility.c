/*
 * File:    Ex_23_sort_utility.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program is a utility wrapper that takes a filename as a
 *   command-line argument and executes the Unix `sort -r` command on
 *   that file to display its contents in reverse sorted order. It
 *   includes basic input validation to ensure exactly one argument is
 *   provided and that the filename is not too long to prevent buffer
 *   overflow.
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
    char command[MAXSTRING];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(1);
    }

    /* guard against filenames that would overflow the buffer */
    if (strlen(argv[1]) > MAXSTRING - 10) {
        fprintf(stderr, "Error: filename too long\n");
        exit(1);
    }

    snprintf(command, MAXSTRING, "sort -r %s", argv[1]);
    system(command);
    return EXIT_SUCCESS;
}