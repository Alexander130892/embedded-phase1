/*
 * File:    fct.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file defines three mathematical functions (f1, f2, f3) that
 *   represent different equations for which roots can be found using
 *   numerical methods. The functions include a cubic polynomial, a
 *   trigonometric-polynomial hybrid, and an exponential-polynomial
 *   combination.
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