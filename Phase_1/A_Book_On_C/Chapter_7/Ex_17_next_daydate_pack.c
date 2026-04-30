/*
 * File:    Ex_17_next_daydate_pack.c
 * Author:  Alexander130892
 * Date:    27-3-2026
 *
 * Description:
 *   This C program demonstrates date packing by compressing day,
 *   month, and year values into a single 16-bit unsigned short integer
 *   using bit manipulation, then implements a next_day function to
 *   calculate the following day. The program takes a date input, packs
 *   it into compressed format, unpacks it to verify the process,
 *   calculates the next day, and displays all results.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

const char* g_months [13] = {"ILLEGAL","January","February","March","April","May","June","July","August","September","October","November","December"};
const int g_days_in_month [13] = {-1,31,28,31,30,31,30,31,31,30,31,30,31};

//Typedefs
unsigned short pack_date(char day, char month, char year);
void unpack_date(unsigned short packed, int unpacked[3]);
unsigned short next_day(unsigned short packed);

//MAIN
int main(void){ //user input not proteced
    int day=0;
    int month=0;
    int year=0;
    printf("give the date: (dd MM YY):\n");
    scanf("%d%d%d", &day, &month, &year);
    printf("The date before packing is: %d/%d/%d\n", day, month, year);
    unsigned short packed = pack_date(day, month, year);
    int unpacked[3];
    unpack_date(packed, unpacked); 
    printf("The date after packing is: %d/%d/%d\n", unpacked[0], unpacked[1], unpacked[2]);
    packed = next_day(packed);
    unpack_date(packed, unpacked);
    printf("The next date after packing is: %d/%d/%d\n", unpacked[0], unpacked[1], unpacked[2]);

    return EXIT_SUCCESS;
}

unsigned short pack_date(char day, char month, char year){
    unsigned short packed = 0;
    packed = packed | day; //5 bits to store day
    packed = (packed << 4) | month; //4 bits to store month
    packed = (packed << 7) | year; //7 bits to store year
    return packed;
}

void unpack_date(unsigned short packed, int date[3]){
    unsigned char year_mask = 0x7f;
    unsigned char month_mask = 0xf;
    unsigned char day_mask = 0x1f;
    date[2]=packed & year_mask;
    date[1]=(packed >> 7) & month_mask;
    date[0]=(packed >> (7+4)) & day_mask;
    return;
}

unsigned short next_day(unsigned short packed) {
    /* unpack */
    int day   = (packed >> 11) & 0x1f;
    int month = (packed >> 7)  & 0x0f;
    int year  =  packed        & 0x7f;

    /* increment */
    int max_days = g_days_in_month[month];


    day++;
    if (day > max_days) {
        day = 1;
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }

    /* repack */
    unsigned short result = 0;
    result = result | day;
    result = (result << 4) | month;
    result = (result << 7) | year;
    return result;
}
