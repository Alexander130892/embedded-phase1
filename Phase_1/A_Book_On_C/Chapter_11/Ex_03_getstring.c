/*
 * File:    Ex_03_getstring.c
 * Author:  Alexander130892
 * Date:    11-4-2026
 *
 * Description:
 *   This program reads the first two lines from an input file
 *   specified as a command line argument, displays them on the
 *   console, and writes them to an output file named "output.txt". It
 *   uses dynamic memory allocation to store the strings and includes
 *   custom functions for reading and writing strings character by
 *   character.
 */
#include <stdio.h>
#include <stdlib.h>


#define BUFFERSIZE 255
char* getstring(FILE *ifp);
void putstring(FILE *ofp, char* str);
void prn_info(char *);

int main(int argc, char **argv)
{
    FILE *ifp, *ofp;

    /* expect: program filename */
    if (argc != 2) {
        prn_info(argv[0]);
        exit(1);
    }

    ifp = fopen(argv[1], "r");
    if (ifp == NULL) {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[1]);
        exit(1);
    }
    ofp = fopen("output.txt", "w");
    if (ofp == NULL) {
        fprintf(stderr, "Error: cannot open '%s'\n", "output.txt");
        exit(1);
    }
    char* in_1=getstring(ifp);
    char* in_2=getstring(ifp);
    printf("%s",in_1);
    printf("%s",in_2);
    putstring(ofp, in_1);
    putstring(ofp, in_2);
    free(in_1);
    free(in_2);
    fclose(ifp);
    fclose(ofp);
    return EXIT_SUCCESS;
}

char* getstring(FILE *ifp){
    int c;
    int index=0;
    char* str= malloc(sizeof(char)*BUFFERSIZE);
    while ((c = getc(ifp)) != EOF && c != '\n') {
        str[index]=c;
        index++;
    }
    if (c == '\n')
        str[index++] = '\n';   // only add it if we actually read one
    str[index] = '\0';
    return str;
}
void putstring(FILE *ofp, char* str){
    int c;
    int index=0;
    while ((c=str[index++]) != '\0') {
        putc(c,ofp);
    }
}

void prn_info(char * pgm_name)
{
    fprintf(stderr, "\n%s%s%s\n","Usage: ", pgm_name, " infile");
}