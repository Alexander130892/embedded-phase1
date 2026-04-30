/*
 * File:    Ex_04_linenrs.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program reads a text file specified as a command-line
 *   argument and prints its contents to stdout with line numbers
 *   prefixed to each line. It uses left-aligned 3-digit formatting for
 *   the line numbers and handles newlines by incrementing the line
 *   counter and starting a new numbered line.
 */
#include <stdio.h>
#include <stdlib.h>


#define BUFFERSIZE 255

void putlinenr(FILE *ifp);

int main(int argc, char **argv)
{
    FILE *ifp;
    /* expect: program filename */

    ifp = fopen(argv[1], "r");
    if (ifp == NULL) {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[1]);
        exit(1);
    }
    putlinenr(ifp);
    fclose(ifp);
    return EXIT_SUCCESS;
}
void putlinenr(FILE *ifp){
    int c;
    int line=1;
    char str[256];
    fprintf(stdout, "\n%-3d ", line);
    while ((c = getc(ifp)) != EOF) {
        if(c=='\n'){
            line++;
            fprintf(stdout, "\n%-3d ", line);
        }
        else
            fprintf(stdout, "%c", c);
    }
    putchar('\n');
}