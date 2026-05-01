/*
 * File:    Ex_08_search.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program searches for a specified string within a text file
 *   and prints all lines containing that string to standard output. It
 *   takes two command-line arguments (search string and filename) and
 *   uses the strstr() function to perform case-sensitive substring
 *   matching on each line of the file.
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