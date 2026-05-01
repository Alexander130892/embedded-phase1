/*
 * File:    Ex_11_wrt_rand.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This C program prompts the user for a filename and number count,
 *   then generates that many random integers and writes them to the
 *   specified file, formatting 6 numbers per line. It includes safety
 *   checks to prevent accidental file overwrites and handles file
 *   opening errors gracefully.
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

FILE* gfopen(char* fname, char* mode);
FILE* cfopen(char* fname, char* mode);
void get_info(char* fname, int* n_ptr);
int main(void)
{   
    int n;
    char* fname;
    get_info(fname, &n);
    FILE* ofp=cfopen(fname,"w");
    srand(time(NULL));
    for(int i = 1; i <= n ; i++ )
    {
        fprintf(ofp, "%12d", rand());
        if(i%6==0 || i ==n)
            fprintf(ofp, "\n");
    }
    return 0;
}

void get_info(char* fname, int* n_ptr)
{
    printf("\n%s\n\n%s", "This program creates a file od random numbers.",
                        "How many random numbers would you like?    ");
    scanf("%d", n_ptr);
    printf("\nIn what file would you like these?    ");
    scanf("%s", fname);
}

FILE* cfopen(char* fname, char* mode)
{
    char reply[2];
    FILE *fp;

    if((strcmp(mode, "w") == 0) && (access(fname, F_OK) == 0))
    {
        printf("\nFile exists. Overwrite it?   ");
        scanf("%1s", reply);
        if(*reply != 'y' && *reply != 'Y')
        {
            printf("\nBye!\n\n");
            exit(1);
        }
    }
    fp = gfopen(fname, mode);
    return fp;
}

FILE* gfopen(char* fname, char* mode)
{
    FILE *fp;
    if((fp=fopen(fname, mode))==NULL)
    {
        fprintf(stderr, "Cannot open %s - bye!", fname);
        exit(1);
    }
    return fp;
}