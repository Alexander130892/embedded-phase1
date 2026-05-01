/*
 * File:    Ex_05_ungetc.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates the `ungetc()` function by reading the
 *   first three characters from a file, displaying them, then using
 *   `ungetc()` to push them back onto the input stream in reverse
 *   order, and finally reading three characters again to show they
 *   were successfully returned to the stream. The program includes an
 *   unused `putlinenr()` function that would add line numbers to file
 *   output.
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
    int c1=getc(ifp);
    int c2=getc(ifp);
    int c3=getc(ifp);
    fprintf(stdout,"%c", c1);
    fprintf(stdout,"%c", c2);
    fprintf(stdout,"%c\n", c3);

    fprintf(stdout,"%c",ungetc(c1,ifp));
    fprintf(stdout,"%c",ungetc(c2,ifp));
    fprintf(stdout,"%c\n",ungetc(c3,ifp));

    fprintf(stdout,"%c",getc(ifp));
    fprintf(stdout,"%c",getc(ifp));
    fprintf(stdout,"%c\n",getc(ifp));
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