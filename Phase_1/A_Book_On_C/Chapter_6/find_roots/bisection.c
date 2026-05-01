/*
 * File:    bisection.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file implements the bisection method for finding roots of a
 *   function by recursively narrowing down an interval where the
 *   function changes sign until the root is found within a specified
 *   tolerance.
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