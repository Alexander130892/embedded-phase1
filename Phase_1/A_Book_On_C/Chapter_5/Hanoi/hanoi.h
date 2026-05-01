/*
 * File:    hanoi.h
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This header file declares the interface for a Tower of Hanoi
 *   solver, providing functions to get the number of disks from user
 *   input and recursively move disks between three pegs, along with a
 *   global counter variable to track operations.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

extern long cnt;

int get_n_from_user(void);
void move(int n, char a, char b, char c);