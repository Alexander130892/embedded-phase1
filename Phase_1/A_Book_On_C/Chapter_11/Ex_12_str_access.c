/*
 * File:    Ex_12_str_access.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This C program demonstrates different string access methods by
 *   reading characters from a string using sscanf with a fixed pointer
 *   versus an incrementing pointer, writing the results to two
 *   separate files to show that the fixed pointer repeatedly reads the
 *   same character while the incrementing pointer reads each character
 *   sequentially.
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int main(void)
{   
    char c, s[]="abc", *p = s;
    int i;
    FILE* ofp1, *ofp2;

    ofp1=fopen("tmp1", "w");
    ofp2=fopen("tmp2", "w");
    for(i=0;i<3;i++){
        sscanf(s, "%c", &c);
        fprintf(ofp1, "%c", c);
    }
    for(i=0;i<3;i++){
        sscanf(p++, "%c", &c);
        fprintf(ofp2, "%c", c);
    }
    return 0;
}

