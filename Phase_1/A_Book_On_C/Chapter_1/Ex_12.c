/*
 * File:    Ex_12.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program generates and displays a formatted table showing
 *   integers from 1 to 10 alongside their corresponding 2nd, 3rd, 4th,
 *   and 5th powers. The output is neatly organized in columns with
 *   headers and separators for clear readability.
 */
#include <stdio.h>
#include <stdlib.h>

#define LIMIT 10

int main(void){
    printf(":::::\tA TABLE OF POWERS\t:::::\n");
    printf("%s%15s%15s%15s%15s","Integer", "Square", "3rd Power", "4th Power", "5th Power");
    printf("\n%s%15s%15s%15s%15s","-------", "------", "---------", "---------", "---------");
    for(int i=1; i<= LIMIT; i++){
        printf("\n%7d%15d%15d%15d%15d",i, i*i, i*i*i, i*i*i*i, i*i*i*i*i);
    }
    printf("\n");
    return EXIT_SUCCESS;
}