/*
 * File:    dbl_space.c
 * Author:  Alexander130892
 * Date:    11-4-2026
 *
 * Description:
 *   This file implements a text formatting utility that reads a file
 *   and outputs it with adjustable line spacing (single through
 *   quintuple spacing), defaulting to double spacing. It skips
 *   consecutive blank lines in the input and allows spacing control
 *   via command-line options (-1 through -5).
 */
#include <stdio.h>
#include <stdlib.h>

void prn_info(char *pgm_name);
void n_space(FILE *ifp, FILE *ofp, int n);

int main(int argc, char **argv)
{
    FILE *ifp;
    int spacing = 2;   /* default: double space */
    int file_arg = -1;

    /* expect: program [-n] filename */
    if (argc < 2 || argc > 3) {
        prn_info(argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int n = atoi(&argv[i][1]);   /* parse digit after '-' */
            if (n < 1 || n > 5) {
                fprintf(stderr, "Invalid option '%s'. Use -1 through -5.\n", argv[i]);
                exit(1);
            }
            spacing = n;
        } else {
            file_arg = i;
        }
    }

    if (file_arg == -1) {
        prn_info(argv[0]);
        exit(1);
    }

    ifp = fopen(argv[file_arg], "r");
    if (ifp == NULL) {
        fprintf(stderr, "Error: cannot open '%s'\n", argv[file_arg]);
        exit(1);
    }

    n_space(ifp, stdout, spacing);
    fclose(ifp);
    return EXIT_SUCCESS;
}

void n_space(FILE *ifp, FILE *ofp, int n)
{
    int c;
    int prev = 0;

    while ((c = getc(ifp)) != EOF) {
        if (c == '\n') {
            if (prev == '\n')   /* skip consecutive newlines */
                continue;
            putc(c, ofp);
            for (int i = 1; i < n; i++)   /* add n-1 extra newlines */
                putc('\n', ofp);
        } else {
            putc(c, ofp);
        }
        prev = c;
    }
}

void prn_info(char *pgm_name)
{
    fprintf(stderr, "Usage: %s [-1|-2|-3|-4|-5] filename\n", pgm_name);
    fprintf(stderr, "  -1  single space\n");
    fprintf(stderr, "  -2  double space (default)\n");
    fprintf(stderr, "  -3  triple space\n");
    fprintf(stderr, "  -4  quadruple space\n");
    fprintf(stderr, "  -5  quintuple space\n");
}