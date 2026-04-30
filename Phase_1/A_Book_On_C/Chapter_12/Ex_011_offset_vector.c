/*
 * File:    Ex_011_offset_vector.c
 * Author:  Alexander130892
 * Date:    15-4-2026
 *
 * Description:
 *   This file demonstrates pointer arithmetic by creating an offset
 *   array where the pointer is shifted backward so that array indices
 *   correspond to years (1989-1998), allowing direct indexing by year
 *   rather than starting from 0.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

double trace(double **, int);

int main(void)
{
    
    int * p_automobiles = calloc(10, sizeof(int));
    p_automobiles -= 1989;
    for(int i=1989 ; i< 1999; i++){
        p_automobiles[i] = 2*i;
    }
    for(int i=1989 ; i< 1999; i++){
        printf("Cars sold in %d:\t%d\n", i, p_automobiles[i]);
    }
    free(p_automobiles+1989);
    return 0;
}

