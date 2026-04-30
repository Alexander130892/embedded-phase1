/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This file serves as the main driver program that tests a bisection
 *   root-finding algorithm on three different functions (f1, f2, f3)
 *   within the interval [-100, 100]. It counts and displays the number
 *   of function calls made during each root-finding operation and
 *   prints the approximate roots and their corresponding function
 *   values with high precision.
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