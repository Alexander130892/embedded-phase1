/*
 * File:    Ex_14_enum_month.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This C program demonstrates the use of enums by defining months of
 *   the year and displaying each month alongside its previous month,
 *   with special handling to wrap December to January. It prints a
 *   formatted table showing current and previous month pairs for all
 *   12 months.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//Typedefs
const int g_start_budget = 5;
int g_plays=0;

typedef enum{
    January,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
} Month;

const char* months [12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};

//Prototype Functions
Month previous_month(Month current);

//MAIN
int main(void){
    Month previous;
    printf("%s\t%s\n","Current", "Previous");
    for (Month current = January ; current <= December ; current++){
        previous = previous_month(current);
        printf("%s\t%s\n",months[current], months[previous]);
    }
    return EXIT_SUCCESS;
}

Month previous_month(Month current){
    Month previous;
    if(current == January)
        previous=December;
    else
        previous=current-1;
    return previous;
}

