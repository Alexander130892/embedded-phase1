/*
 * File:    Ex_09_search_plus.c
 * Author:  Alexander130892
 * Date:    11-4-2026
 *
 * Description:
 *   This program searches for a specified string within a text file
 *   and prints matching lines, with an optional -n flag to include
 *   line numbers in the output. It handles long lines by truncating
 *   them at 255 characters while warning the user and ensuring logical
 *   line counting remains accurate.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 255

void prn_info(char *pgm_name);
void searchstr(FILE *ifp, char *search, int line_option);
int is_n_option(char *arg);

int main(int argc, char **argv)
{
    FILE *ifp;
    int line_option = 0;
    char *search;
    char *filename;

    /* valid combinations: program search file
                           program -n search file */
    if (argc != 3 && argc != 4) {
        prn_info(argv[0]);
        exit(1);
    }

    if (argc == 4) {
        if (!is_n_option(argv[1])) {
            fprintf(stderr, "Error: unknown option '%s'\n", argv[1]);
            prn_info(argv[0]);
            exit(1);
        }
        line_option = 1;
        search   = argv[2];
        filename = argv[3];
    } else {
        if (is_n_option(argv[1])) {   /* -n given but missing filename */
            fprintf(stderr, "Error: missing filename after -n\n");
            prn_info(argv[0]);
            exit(1);
        }
        line_option = 0;
        search   = argv[1];
        filename = argv[2];
    }

    /* guard against empty search string */
    if (strlen(search) == 0) {
        fprintf(stderr, "Error: search string cannot be empty\n");
        exit(1);
    }

    ifp = fopen(filename, "r");
    if (ifp == NULL) {
        fprintf(stderr, "Error: cannot open '%s'\n", filename);
        exit(1);
    }

    searchstr(ifp, search, line_option);
    fclose(ifp);
    return EXIT_SUCCESS;
}

int is_n_option(char *arg)
{
    /* valid only if exactly "-n", nothing else */
    return (arg[0] == '-' && arg[1] == 'n' && arg[2] == '\0');
}

void searchstr(FILE *ifp, char *search, int line_option)
{
    char line[MAX_LINE];
    int linenr = 1;
    int truncated = 0;   /* flag: current logical line was truncated */

    while (fgets(line, MAX_LINE, ifp) != NULL) {
        int has_newline = (strchr(line, '\n') != NULL);

        if (!truncated) {
            /* only report on first chunk of a logical line */
            if (strstr(line, search) != NULL) {
                if (!line_option)
                    printf("%s", line);
                else
                    printf("%3d: %s", linenr, line);

                if (!has_newline)
                    printf("\n");   /* ensure output ends on a new line */
            }
        }

        /* if no newline was read, the line was longer than MAX_LINE */
        if (!has_newline) {
            if (!truncated) {
                fprintf(stderr, "Warning: line %d exceeds %d chars, truncated\n",
                        linenr, MAX_LINE - 1);
            }
            truncated = 1;
        } else {
            if (truncated)
                linenr++;   /* count the logical line, not the chunk */
            truncated = 0;
            linenr++;
        }
    }
}

void prn_info(char *pgm_name)
{
    fprintf(stderr, "Usage: %s [-n] search filename\n", pgm_name);
    fprintf(stderr, "  -n   prefix matching lines with line numbers\n");
}