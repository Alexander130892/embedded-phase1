/*
 * File:    Ex_33_dissection.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file demonstrates C array and pointer manipulation by
 *   creating a 3x5 integer array, filling it with sequential values,
 *   and displaying the data through different access methods including
 *   2D array notation, pointer arithmetic, and various dereference
 *   operations. The file also contains unused helper functions for
 *   drawing coordinate axes on a character screen, suggesting it may
 *   be part of a larger graphics or plotting exercise.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Consts
#define COLS  80
#define ROWS  24
#define PI    3.14159265358979323846

//Typedefs

//Prototype Functions
void plot(char screen[ROWS][COLS], int row, int col, char c);
void draw_axes(char screen[ROWS][COLS]);
void print_screen(char screen[ROWS][COLS]);

//MAIN
int main(){
    int a[3][5], i, j, *p=*a;

    for (i=0; i < 3; i++)
    {
        for (j = 0; j < 5; j++)
        {
            a[i][j]=i*5+j;
        }
    }

    for (i=0; i < 3; i++)
    {
        for (j = 0; j < 5; j++)
        {
            printf("%s%12d", (j == 0) ? "\n" : "", a[i][j]);
        }
    }
    putchar('\n');

    for (i=0; i < 15; i++)
    {
        printf("%s%12d", (i % 5 == 0) ? "\n" : "", *(p+i));
    }
    putchar('\n');

    printf("\n%12d%12d\n%12d%12d\n%12d%12d\n%12d%12d\n\n", 
            **a, **(a+1), 
            *(a[0]+1), *(*a+1),
            *(a[1]+2), *(*(a+1)+2),
            *(a[2]+3), *(*(a+2)+3));

    printf("\n%--11s%s%12d\n%--11s%s%12d\n%--11s%s%12d\n\n", 
            "(int) a", " = ", (int) a,
            "(int) *a", " = ", (int) *a,
            "(int) **a", " = ", (int) **a);
            // a      → pointer to row 0 (type int(*)[5])
            // *a     → a[0], pointer to a[0][0]
            // **a    → a[0][0] = 0

    return EXIT_SUCCESS;
}

//Helper-Functions
void plot(char screen[ROWS][COLS], int row, int col, char c){
    if(row >= 0 && row < ROWS && col >= 0 && col < COLS)
        screen[row][col] = c;
}

void draw_axes(char screen[ROWS][COLS]){
    int mid_row = ROWS / 2;
    int mid_col = COLS / 2;

    /* x-axis */
    for(int c = 0; c < COLS; c++)
        screen[mid_row][c] = '-';

    /* y-axis */
    for(int r = 0; r < ROWS; r++)
        screen[r][mid_col] = '|';

    /* origin */
    screen[mid_row][mid_col] = '+';
}

void print_screen(char screen[ROWS][COLS]){
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLS; c++)
            putchar(screen[r][c]);
        putchar('\n');
    }
}