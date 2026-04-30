/*
 * File:    dbl_space.c
 * Author:  Alexander130892
 * Date:    11-4-2026
 *
 * Description:
 *   This file reads a text file specified as a command-line argument
 *   and outputs its contents to stdout with double spacing by
 *   inserting an additional newline character after each existing
 *   newline. If the wrong number of arguments is provided, it prints
 *   usage information to stderr and exits.
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