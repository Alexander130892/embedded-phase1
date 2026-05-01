/*
 * File:    Ex_08_search.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements a simple grep-like utility that searches for
 *   a specified string within a text file and prints all lines
 *   containing that string. The program takes a search string and
 *   filename as command-line arguments, reads the file line by line,
 *   and outputs any lines that contain the search string.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 255

void prn_info(char *pgm_name);
void searchstr(FILE *ifp, char* search);

int main(int argc, char **argv)
{
    FILE *ifp;
    

    if (argc != 3) {
        prn_info(argv[0]);
        exit(1);
    }
    char * search = argv[1];
    ifp = fopen(argv[2], "r");
    if (ifp == NULL) {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[1]);
        exit(1);
    }

    searchstr(ifp, search);

    fclose(ifp);
    return EXIT_SUCCESS;
}

void searchstr(FILE *ifp, char* search){
    char line[MAX_LINE];
    while(fgets(line, MAX_LINE, ifp) != NULL)
    {
        if(strstr(line, search) != NULL)
        {
            printf("%s",line);
        }
    }
}

void prn_info(char *pgm_name)
{
    fprintf(stderr, "Usage: %s search filename\n", pgm_name);
}