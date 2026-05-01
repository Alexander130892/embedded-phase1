/*
 * File:    Ex_07_multiple_screens.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements a command-line file pager that displays one
 *   or more text files with pagination, showing a specified number of
 *   lines per page (default 20) and waiting for user input to
 *   continue. It supports a -n option to customize lines per page and
 *   clears the screen between multiple files.
 */
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_LINES 20

void prn_info(char *pgm_name);
void pager(FILE *ifp, int lines_per_page);

int main(int argc, char **argv)
{
    FILE *ifp;
    int lines_per_page = DEFAULT_LINES;
    int first_file = 1;   /* index of first filename in argv */

    if (argc < 2) {
        prn_info(argv[0]);
        exit(1);
    }

    /* check for -n option */
    if (argv[1][0] == '-') {
        int n = atoi(&argv[1][1]);
        if (n < 1) {
            fprintf(stderr, "Invalid option '%s'. Use -n where n >= 1.\n", argv[1]);
            exit(1);
        }
        lines_per_page = n;
        first_file = 2;   /* filenames start after the option */
    }

    if (first_file >= argc) {
        prn_info(argv[0]);
        exit(1);
    }

    /* loop over all file arguments */
    for (int i = first_file; i < argc; i++) {
        if (i > first_file) {
            system("clear");
            printf("=== File: %s ===\n\n", argv[i]);
        }

        ifp = fopen(argv[i], "r");
        if (ifp == NULL) {
            fprintf(stderr, "Error: cannot open '%s'\n", argv[i]);
            continue;   /* skip bad file, try next */
        }

        printf("=== File: %s ===\n\n", argv[i]);
        pager(ifp, lines_per_page);
        fclose(ifp);
    }

    return EXIT_SUCCESS;
}

void pager(FILE *ifp, int lines_per_page)
{
    int c;
    int line_count = 0;

    while ((c = getc(ifp)) != EOF) {
        putchar(c);

        if (c == '\n') {
            line_count++;

            if (line_count == lines_per_page) {
                printf("--- press ENTER for next page (q to quit) ---");

                int input = getchar();
                if (input == 'q' || input == 'Q')
                    return;   /* stop this file, move to next */

                /* flush any remaining input on the line */
                while (input != '\n' && input != EOF)
                    input = getchar();

                line_count = 0;
            }
        }
    }
}

void prn_info(char *pgm_name)
{
    fprintf(stderr, "Usage: %s [-n] file1 [file2 ...]\n", pgm_name);
    fprintf(stderr, "  -n   lines per page (default: %d)\n", DEFAULT_LINES);
}