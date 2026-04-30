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

void double_space(FILE *, FILE *);
void prn_info(char *);

int main(int argc, char** argv)
{
    FILE *ifp, *ofp;

    if(argc!= 2){
        prn_info(argv[0]);
        exit(1);
    }
    ifp = fopen(argv[1], "r");
    ofp = stdout;
    double_space(ifp, ofp);
    fclose(ifp);
    return EXIT_SUCCESS;
}

void double_space(FILE* ifp, FILE * ofp)
{
    int c;
    while ((c=getc(ifp))!=EOF)
    {
        putc(c, ofp);
        if(c=='\n')
            putc('\n', ofp);
    }
}
void prn_info(char * pgm_name)
{
    fprintf(stderr, "\n%s%s%s\n\n%s%s\n\n",
        "Usage: ", pgm_name, " infile","The content of infile will be double-spaced", 
        " and written to stdout.");
}