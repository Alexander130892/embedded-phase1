/*
 * File:    Test_theory.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file benchmarks the performance of the rand() function by
 *   timing 100 million calls to it, displaying a sample of the first
 *   24 generated random numbers (modulo 101) in a 3x8 grid format, and
 *   calculating the average time per function call.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Consts
#define NCALLS 100000000 // number of fct calls
#define NCOLS 8 // number of collumns
#define NLINES 3 // number of lines


//Typedefs

//Prototype Functions

//MAIN
int main(){
    int i,val;
    long begin, diff,end;

    begin=time(NULL);
    srand(time(NULL));
    printf("\nTIMING TEST: %d calls to rand()\n\n", NCALLS);
    for (i=1; i<= NCALLS ; ++i){
        val=rand();
        if (i<= NCOLS * NLINES){
            printf("%7d ", val%101);
            if (i%NCOLS==0)
                putchar('\n');
        }
        else if (i == NCOLS * NLINES +1)
            printf("%7s\n\n", ".....");
    }
    end= time(NULL);
    diff = end - begin;
    printf("%s%ld\n%s%ld\n%s%ld\n%s%.10f\n\n",
        "        end time: ", end,
        "      begin time: ", begin, 
        "    elapsed time: ", diff,
        "time for each call: ", (double) diff / NCALLS);
    return EXIT_SUCCESS;
}

//Helper-Functions