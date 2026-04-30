/*
 * File:    Ex_36_birthdaygame copy.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program simulates the famous birthday paradox problem by
 *   running Monte Carlo simulations to calculate the probability that
 *   at least two people share the same birthday in groups of varying
 *   sizes (2 to 49 people). It performs one million trials for each
 *   group size and outputs the success rate as a percentage.
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
    srand48(time(NULL));
    int hit=0;
    for(int n=2;n<50;n++){
        hit=0;
        for (int i=0;i<COUNT;i++){
            int days[365]={0};
            for(int p=0;p<n;p++){
                days[lrand48()%365]++;
            }
            for(int k=0; k<365;k++){
                if(days[k]>1){
                    hit++;
                    break;
                } 
            }
            
        }
    printf("Average Succesrate for %d persons: %.2lf%% \n", n, (double)hit/COUNT*100);   
}
    return 0;
}