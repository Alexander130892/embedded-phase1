/*
 * File:    Ex_03_execl_overlay.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file demonstrates process overlay using execl() by prompting
 *   the user to choose between three different programs (pgm1, pgm2,
 *   or pgm3) and then replacing the current process with the selected
 *   program using different command-line arguments. The execl() calls
 *   overlay the current process entirely, so any code after the
 *   execl() calls will not execute unless the execl() fails.
 */
// Compute Fibonacci numbers and print time asynchronously

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>


int main(void)
{
    int choice = 0;

    printf("%s\n%s\n%s",
        "The parent process will be overlaid.",
        "You have a choice.",
        "Input 1, 2 or 3: ");
    scanf("%d", &choice);
    putchar('\n');
    switch (choice)
    {
    case 1:
        execl("pgm1", "pgm1", "a", "b", "c", 0);
        break;
    case 2:
        execl("pgm2", "pgm2", "1", "2", "3", "go", 0);
        break;
    case 3:
        execl("pgm3", "pgm3", 0);
        break;
    default:
        printf("You did not put in 1, 2 or 3\n");
        break;
    }
    return EXIT_SUCCESS;
}
