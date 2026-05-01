/*
 * File:    Ex_12_str_access.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This program demonstrates the difference between using a string
 *   array name versus a pointer when accessing string characters,
 *   writing the first character 'a' three times to "tmp1" (since the
 *   array name `s` doesn't advance) and the full string "abc" to
 *   "tmp2" (since the pointer `p` increments through each character).
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

