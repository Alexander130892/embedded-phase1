/*
 * File:    hanoi.h
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This header file declares functions and a global counter for
 *   implementing the Tower of Hanoi puzzle, including user input for
 *   the number of disks and the recursive move function that transfers
 *   disks between three pegs.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

extern long cnt;

int get_n_from_user(void);
void move(int n, char a, char b, char c);