/*
 * File:    get.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file prompts the user to input the number of disks for a
 *   Towers of Hanoi puzzle and validates that the input is a positive
 *   integer, terminating the program with an error message if the
 *   input is invalid.
 */

#include "hanoi.h"

int get_n_from_user(){
    int n;
    printf("%s", "TOWERS OF HANOI:\n"
        "There are 3 towers: A, B, C\n"
        "n disks need to be moved from tower A to C\n"
        "Input n: ");
    if(scanf("%d",&n) !=1 || n<0){
        printf("ERROR: n needs to be positive!\n");
        exit(EXIT_FAILURE);
    }
    printf("\n");
    return n;
}