/*
 * File:    Ex_23_sort_utility.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements a command-line utility that takes a filename
 *   as an argument and executes the Unix `sort -r` command on that
 *   file to display its contents in reverse sorted order. It includes
 *   basic error checking for argument count and filename length to
 *   prevent buffer overflow when constructing the system command.
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