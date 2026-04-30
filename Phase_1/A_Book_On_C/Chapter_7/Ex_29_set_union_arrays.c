/*
 * File:    Ex_24_condensed_pack.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program demonstrates bit manipulation by packing four
 *   individual char values into a single unsigned integer using left
 *   shifts and bitwise OR operations, then prints the resulting bit
 *   pattern with spaces between each byte.
 */
//write pack into one line
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BITS_PER_WORD   (sizeof(unsigned int) * CHAR_BIT)  /* 32 */
#define NWORDS(n)       (((n) + BITS_PER_WORD - 1) / BITS_PER_WORD)
#define WORD(i)         ((i) / BITS_PER_WORD)
#define BIT(i)          ((i) % BITS_PER_WORD)

unsigned int *set_create(int n) {
    return calloc(NWORDS(n), sizeof(unsigned int));  /* zero initialised */
}

void set_free(unsigned int *s) {
    free(s);
}

void set_add(unsigned int *s, int i) {
    s[WORD(i)] |= (1U << BIT(i));
}

void set_remove(unsigned int *s, int i) {
    s[WORD(i)] &= ~(1U << BIT(i));
}

int set_contains(unsigned int *s, int i) {
    return (s[WORD(i)] >> BIT(i)) & 1;
}

/* result must be pre-allocated by caller */
void set_union(unsigned int *a, unsigned int *b, unsigned int *result, int n) {
    for (int w = 0; w < NWORDS(n); w++)
        result[w] = a[w] | b[w];
}

void set_intersection(unsigned int *a, unsigned int *b, unsigned int *result, int n) {
    for (int w = 0; w < NWORDS(n); w++)
        result[w] = a[w] & b[w];
}

void set_complement(unsigned int *a, unsigned int *result, int n) {
    for (int w = 0; w < NWORDS(n); w++)
        result[w] = ~a[w];
}

void display_set(unsigned int *s, int n) {
    printf("The set contains: ");
    for (int i = 0; i < n; i++)
        if (set_contains(s, i))
            printf("%d ", i + 1);
    putchar('\n');
}

int main(void) {
    int n = 1000;   /* works for any n now */

    unsigned int *a      = set_create(n);
    unsigned int *b      = set_create(n);
    unsigned int *result = set_create(n);

    set_add(a, 0);     /* element 1   */
    set_add(a, 1);     /* element 2   */
    set_add(b, 999);   /* element 1000 */

    set_union(a, b, result, n);
    display_set(result, n);

    set_free(a);
    set_free(b);
    set_free(result);
    return EXIT_SUCCESS;
}