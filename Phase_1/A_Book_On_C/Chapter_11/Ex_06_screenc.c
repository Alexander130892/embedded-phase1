/*
 * File:    Ex_06_screenc.c
 * Author:  Alexander130892
 * Date:    11-4-2026
 *
 * Description:
 *   This file implements a simple pager utility that displays a text
 *   file 20 lines at a time, prompting the user to press ENTER to
 *   continue to the next page or 'q' to quit. It takes a filename as a
 *   command-line argument and provides basic error handling for file
 *   opening and usage.
 */
#include <stdio.h>
#include <stdlib.h>

#define LINES_PER_PAGE 20

void prn_info(char *pgm_name);
void pager(FILE *ifp);

int main(int argc, char **argv)
{
    FILE *ifp;

    if (argc != 2) {
        prn_info(argv[0]);
        exit(1);
    }

    ifp = fopen(argv[1], "r");
    if (ifp == NULL) {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[1]);
        exit(1);
    }

    pager(ifp);

    fclose(ifp);
    return EXIT_SUCCESS;
}

void pager(FILE *ifp)
{
    int c;
    int line_count = 0;

    while ((c = getc(ifp)) != EOF) {
        putchar(c);

        if (c == '\n') {
            line_count++;

            if (line_count == LINES_PER_PAGE) {
                printf("--- press ENTER for next page (q to quit) ---");
                
                int input = getchar();         /* read from keyboard */
                if (input == 'q' || input == 'Q')
                    break;

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
    fprintf(stderr, "Usage: %s filename\n", pgm_name);
}