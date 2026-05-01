/*
 * File:    Ex_25_birthdaygame.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This program simulates the birthday paradox by running Monte Carlo
 *   simulations to calculate the probability that at least two people
 *   in groups of 2-20 share the same birth month. It performs one
 *   million trials for each group size and prints the success rate as
 *   a percentage.
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