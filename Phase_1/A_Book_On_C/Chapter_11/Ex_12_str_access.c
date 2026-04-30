/*
 * File:    Ex_12_str_access.c
 * Author:  Alexander130892
 * Date:    11-4-2026
 *
 * Description:
 *   This C program demonstrates different behaviors when accessing
 *   string characters with sscanf, writing "aaa" to tmp1 file (since
 *   sscanf always reads from the start of the string) and "abc" to
 *   tmp2 file (since the pointer is incremented to read each
 *   successive character).
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

