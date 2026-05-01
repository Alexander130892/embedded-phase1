/*
 * File:    bisection.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file implements the bisection method for finding roots of a
 *   function by recursively narrowing down the interval containing the
 *   root until the midpoint is sufficiently close to the actual root.
 *   The function increments a global counter and terminates when
 *   either an exact root is found or the interval width falls below a
 *   specified epsilon threshold.
 */
#include "find_roots.h"

dbl bisection(pfdd f, dbl a, dbl b){
    ++cnt;
    dbl m = (a+b)/2;
    if(f(m) == 0.0 || (b-a) < eps)
        return m;
    else if (f(a) * f(m) < 0.0)
        return bisection(f, a, m);
    else
        return bisection(f, m, b);
}