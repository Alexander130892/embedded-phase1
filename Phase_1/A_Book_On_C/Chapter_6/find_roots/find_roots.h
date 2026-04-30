/*
 * File:    find_roots.h
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This header file declares functions for finding roots of
 *   mathematical functions using the bisection method, including
 *   multiple equivalent function pointer syntax variations and three
 *   predefined test functions (f1, f2, f3).
 */

#include <assert.h>
#include <math.h>
#include <stdio.h>

#define N 4

typedef double dbl;
typedef dbl (*pfdd)(dbl);

extern int cnt;
extern const dbl eps;

dbl bisection(pfdd f, dbl a, dbl b);
dbl bisection(double f(double x), double a, double b);
dbl bisection(double (*)(double ), double , double );
dbl bisection(double (*f)(double ), double , double );
dbl bisection(double (*f)(double x ), double , double );
dbl f1(dbl x);
dbl f2(dbl x);
dbl f3(dbl x);

