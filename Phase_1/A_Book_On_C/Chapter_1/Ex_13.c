/*
 * File:    Ex_13.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This program reads floating-point numbers from input and
 *   calculates running averages using two different methods: an
 *   incremental average formula and a traditional sum-based average,
 *   displaying both results side-by-side to demonstrate their
 *   equivalence. It continues reading numbers until EOF or invalid
 *   input is encountered, showing the count, current item, and both
 *   average calculations for each input.
 */
#include <stdio.h>
#include <stdlib.h>

#define LIMIT 10

int main(void){
    int i;
    double x;
    double avg=0.0;
    double navg;
    double sum=0.0;
    printf("%5s%17s%17s%17s\n%5s%17s%17s%17s\n\n","Count", "Item", "Average", "Naive avg", "-----", "-----", "--------", "----------");
    for(i=1;scanf("%lf", &x) == 1;++i){
        avg += (x-avg)/i;
        sum+=x;
        navg=sum/i;
        printf("%5d%17e%17e%17e\n", i,x, avg, navg);
    }
    return EXIT_SUCCESS;
}