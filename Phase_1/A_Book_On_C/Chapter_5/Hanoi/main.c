/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a Tower of Hanoi solver that calculates and
 *   displays the number of moves required to solve the puzzle, then
 *   estimates how long it would take to move 64 disks at a rate of one
 *   disk per day. It gets the number of disks from the user, performs
 *   the recursive solution while counting moves, and outputs the time
 *   estimate in billions of years.
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