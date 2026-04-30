/*
 * File:    Ex_31_BST_to_orderedArray copy.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This file demonstrates converting a Binary Search Tree (BST) to a
 *   sorted array using in-order traversal, creating a BST from an
 *   input array and then extracting its elements in sorted order. The
 *   program also includes various tree utility functions for counting
 *   nodes, leaves, depth, and specific values within the BST.
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