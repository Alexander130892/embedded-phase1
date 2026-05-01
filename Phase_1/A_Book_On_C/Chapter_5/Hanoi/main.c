/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements a solution to the Tower of Hanoi puzzle,
 *   prompting the user for the number of disks and then calculating
 *   how many moves are required, with a final message estimating it
 *   would take billions of years to move 64 disks at one disk per day.
 */
#include "hanoi.h"

long cnt = 0;

int main(void){
    int n;
    n = get_n_from_user();
    assert(n>0);
    move(n,'A', 'B', 'C'); //move n disks from tower A to C, through tower B

    printf("%.2lf billion years are needed to move 64 disks, one disk per day", (double)cnt/1e9);
    return EXIT_SUCCESS;
}