/*
 * File:    Ex_01_husbandwife.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This program demonstrates a simple implementation of linked data
 *   structures using husband and wife structs that contain pointers to
 *   each other, creating a bidirectional relationship. It initializes
 *   a married couple and prints out their relationship by accessing
 *   one spouse's name through the other's pointer.
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
