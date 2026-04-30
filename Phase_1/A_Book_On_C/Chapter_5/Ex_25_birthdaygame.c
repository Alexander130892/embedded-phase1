/*
 * File:    Ex_25_birthdaygame.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program simulates the birthday paradox problem by calculating
 *   the probability that at least two people share the same birth
 *   month (rather than birth date) in groups of 2 to 20 people using
 *   Monte Carlo simulation with 1 million trials per group size. It
 *   outputs the success rate percentage for each group size, showing
 *   how quickly the probability increases as more people are added to
 *   the group.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define COUNT 1000000.0

//Typedefs

//Prototypes


int main() {
    srand(time(NULL));
    int hit=0;
    for(int n=2;n<21;n++){
        hit=0;
        for (int i=0;i<COUNT;i++){
            int months[12]={0};
            for(int p=0;p<n;p++){
                months[rand()%12]++;
            }
            for(int k=0; k<12;k++){
                if(months[k]>1){
                    hit++;
                    break;
                } 
            }
            
        }
    printf("Average Succesrate for %d persons: %.2lf%% \n", n, (double)hit/COUNT*100);   
}
    return 0;
}