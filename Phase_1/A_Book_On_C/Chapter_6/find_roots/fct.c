/*
 * File:    fct.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file defines three mathematical functions (f1, f2, f3) that
 *   appear to be test cases for a root-finding algorithm, with f1
 *   being a cubic polynomial, f2 combining trigonometric and
 *   polynomial terms, and f3 combining exponential and polynomial
 *   terms.
 */
#include "find_roots.h"

dbl f1(dbl x){
    return (x*x*x - x*x + 2.0*x -2.0);
}
dbl f2(dbl x){
    return (sin(x)- 0.7*x*x*x + 3.0);
}
dbl f3(dbl x){
    return exp(0.13*x) - x*x*x;
}