/*
 * File:    Ex_13.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program reads floating-point numbers from input and displays
 *   a running average calculated using two different methods: an
 *   incremental averaging formula and a traditional
 *   sum-divided-by-count approach. It outputs a formatted table
 *   showing the count, current input value, and both average
 *   calculations for comparison purposes.
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