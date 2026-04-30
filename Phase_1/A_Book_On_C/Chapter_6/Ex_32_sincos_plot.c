/*
 * File:    Ex_32_sincos_plot.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program creates a text-based plot of sine and cosine
 *   functions over one full period (0 to 2π) on an 80x24 character
 *   grid, marking sine values with 'S' and cosine values with 'C'. It
 *   draws coordinate axes and maps the mathematical functions to
 *   screen coordinates, with the y-axis flipped to accommodate the
 *   top-to-bottom row ordering of terminal displays.
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
    char screen[ROWS][COLS];

    /* fill with blanks */
    for(int r = 0; r < ROWS; r++)
        for(int c = 0; c < COLS; c++)
            screen[r][c] = ' ';

    draw_axes(screen);

    for(int c = 0; c < COLS; c++){
        double x   = (2.0 * PI * c) / (COLS - 1);   /* map col → [0, 2PI] */
        double sy  = -sin(x); //y flipped (row 0 = top) */
        double cy  = -cos(x);

        /* map [-1,1] → [0, ROWS-1], */
        int sin_row = ((int)((1.0 - sy) / 2.0 * (ROWS - 1)));
        int cos_row = ((int)((1.0 - cy) / 2.0 * (ROWS - 1)));

        plot(screen, sin_row, c, 'S');
        plot(screen, cos_row, c, 'C');
    }

    print_screen(screen);
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