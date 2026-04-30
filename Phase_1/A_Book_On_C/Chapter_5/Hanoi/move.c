/*
 * File:    move.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements a recursive solution to the Tower of Hanoi
 *   puzzle, printing each step required to move n disks from tower A
 *   to tower C using tower B as auxiliary storage. It uses a global
 *   counter to track and display the sequence number of each move.
 */

#include "hanoi.h"
void move(int n, char A, char B, char C){
    if(n==1){
        cnt++;
        printf("%5ld: %s%d%s%c%s%c.\n", cnt,
            "Move disk ", 1, " from tower ", A, " to tower ",C);
    }else{
        move(n-1, A, C, B);
        cnt++;
        printf("%5ld: %s%d%s%c%s%c.\n", cnt,
            "Move disk ", n, " from tower ", A, " to tower ",C);
        move(n-1, B, A, C);   
    }
}