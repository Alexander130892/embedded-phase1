/*
 * File:    bisection.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file implements the bisection method for finding roots of a
 *   function by recursively narrowing down an interval [a,b] where the
 *   function changes sign, stopping when the function equals zero or
 *   the interval becomes smaller than a specified epsilon tolerance.
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