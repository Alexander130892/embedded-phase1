/*
 * File:    Ex_01_husbandwife.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This C program demonstrates mutual referencing between two
 *   structures by creating husband and wife structs that point to each
 *   other through spouse pointers, then prints a message showing their
 *   relationship.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>


//Consts
struct wife;
    struct husband
    {
        int         age;
        char        name[10];
        struct wife* spouse;
    } a;
    struct wife
    {
        int         age;
        char        name[10];
        struct husband* spouse;
    } b;

int main() {
    
    struct husband h = {35, "Jan"};
    struct wife    w = {32, "An"};
    h.spouse = &w;
    w.spouse = &h;

    printf("%s is married to %s\n", h.name, h.spouse->name);
    return EXIT_SUCCESS;
}
