/*
 * File:    Ex_22_BooleanTruthtable.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This C program generates and displays truth tables for three
 *   different boolean functions operating on 4-bit values (nibbles),
 *   including OR, AND, and a complex boolean expression. It uses a
 *   union structure to access individual bits and systematically
 *   evaluates each function across all 16 possible 4-bit input
 *   combinations.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>

//Consts
typedef union{
    unsigned char raw;
    struct {
        unsigned int b1 :1; //LSB
        unsigned int b2 :1;
        unsigned int b3 :1;
        unsigned int b4 :1; //MSB
    } bits;
} nibble_t;

void print_table(const char* label, const char* fct_label, int (*fct)(nibble_t))
{
    int cnt = 0;
    printf("\n%5s%5s%5s%5s%5s%5s\n", "Cnt", "b1", "b2", "b3", "b4", fct_label);
    for( unsigned char b = 0; b < 16 ; b++)
    {
        nibble_t n = { .raw = b};
        printf("%5d%5d%5d%5d%5d%5d\n", cnt++, n.bits.b1, n.bits.b2, n.bits.b3, n.bits.b4,fct(n));
    }
}

int fct1(nibble_t n) { return n.bits.b1 || n.bits.b2 || n.bits.b3 || n.bits.b4; }
int fct2(nibble_t n) { return n.bits.b1 && n.bits.b2 && n.bits.b3 && n.bits.b4; }
int fct3(nibble_t n) { return !(!n.bits.b1 || n.bits.b2) && (!n.bits.b3 || n.bits.b4); }

int main(){
    print_table("OR  ", "fct1", fct1);
    print_table("AND ", "fct2", fct2);
    print_table("COMPLEX", "fct3", fct3);
    return EXIT_SUCCESS;
    return EXIT_SUCCESS;
}
