/*
 * File:    Ex_32_sincos_plot.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program creates an ASCII text-based plot of sine and cosine
 *   functions over one complete period (0 to 2π) in a 24x80 character
 *   grid. It displays the sine function using 'S' characters and the
 *   cosine function using 'C' characters, with coordinate axes drawn
 *   using dashes, pipes, and a plus sign at the origin.
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