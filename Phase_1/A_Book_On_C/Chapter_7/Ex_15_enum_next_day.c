/*
 * File:    Ex_15_enum_next_day.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This program takes a date input (day and month as integers) and
 *   calculates the next day, handling month transitions when reaching
 *   the end of a month. It uses an enumerated type for months and
 *   lookup tables for month names and days per month, though it
 *   doesn't account for leap years in February.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//Typedefs

typedef enum{
    ILLEGAL,
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
} month;


const char* g_months [13] = {"ILLEGAL","January","February","March","April","May","June","July","August","September","October","November","December"};
const int g_days_in_month [13] = {-1,31,28,31,30,31,30,31,31,30,31,30,31};

//Prototype Functions
void nextday(int* current_day, month* current_month);

//MAIN
int main(void){ //user input not proteced
    int current_day=0;
    month current_month=ILLEGAL;
    printf("give current date: (dd MM):\n");
    scanf("%d%d", &current_day, &current_month);
    printf("The current day is %d %s\n", current_day, g_months[current_month]);
    nextday(&current_day, &current_month);
    printf("The next day is %d %s\n", current_day, g_months[current_month]);

    return EXIT_SUCCESS;
}
void nextday(int* p_current_day, month* p_current_month){
    if( *p_current_day == g_days_in_month[*p_current_month]){
        *p_current_day=1;
        *p_current_month+=1;
    }
    else *p_current_day+=1;
    return;
}


