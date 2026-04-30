/*
 * File:    hanoi.h
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This header file declares functions for solving the Tower of Hanoi
 *   puzzle, including a function to get the number of disks from user
 *   input and a recursive function to perform the disk movements
 *   between three pegs. It also declares an external counter variable,
 *   likely used to track the total number of moves performed.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

extern long cnt;

int get_n_from_user(void);
void move(int n, char a, char b, char c);