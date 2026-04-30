/*
 * File:    Ex_24_cointoss.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program simulates one million coin tosses and calculates the
 *   percentage of heads, the longest streak of consecutive heads, and
 *   the longest streak of alternating results (heads-tails-heads-tails
 *   pattern). It uses random number generation to simulate the coin
 *   flips and tracks various statistical patterns in the results.
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
void hailstones(int n);

int main() {
    srand(time(NULL));
    int heads, prev = -1;
    int total_h  = 0;
    int max_h    = 0, streak_h = 1;
    int max_a    = 0, streak_a = 1;

    for (int i = 0; i < COUNT; i++) {
        heads = rand() % 2;
        //printf("%d ", heads);

        if (heads) total_h++;

        if (prev == -1) {
            /* first flip — just record, no streak logic yet */
        } else if (heads == prev) {
            /* consecutive same value — alternating streak breaks */
            if (streak_a > max_a) max_a = streak_a;
            streak_a = 1;
            streak_h++;
        } else {
            /* alternating — heads streak breaks */
            if (streak_h > max_h) max_h = streak_h;
            streak_h = 1;
            streak_a++;
        }
        prev = heads;
    }

    /* check final streaks */
    if (streak_h > max_h) max_h = streak_h;
    if (streak_a > max_a) max_a = streak_a;

    printf("\nTotal heads: %.2f%%\nMax heads streak: %d\nMax Alt streak: %d\n",
           (double)total_h / COUNT * 100, max_h, max_a);

    return 0;
}