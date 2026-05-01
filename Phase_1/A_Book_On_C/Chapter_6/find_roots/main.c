/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file tests a bisection root-finding algorithm on three
 *   different functions (f1, f2, f3) within the interval [-100, 100],
 *   printing the approximate roots, function values at those roots,
 *   and the number of function calls required for each test case.
 */
#include "find_roots.h"

int cnt =0;
const dbl eps = 1e-13;

int main(){
    int begin_cnt;
    int i;
    int nfct_calls;
    dbl a = -100.0;
    dbl b = 100.0;
    dbl root;
    dbl val;
    pfdd f[N]={NULL, f1, f2, f3};

    for(i=1 ; i<N; i++){
        assert(f[i](a) * f[i](b) <= 0.0);
        begin_cnt = cnt;
        root = bisection(f[i],a,b);
        nfct_calls = cnt - begin_cnt;
        val = f[i](root);
        printf("%s%d%s% .15f\n%s%d%s% .15f\n%s%3d\n\n",
            "For f[",i,"](x) an approximate root is x0 = ", root,
             "Fct evaluation at the root: f[",i,"](x0) = ", val,
            " Number of fct calls to bisection() = ", nfct_calls);
    }
    return 0;
}