/*
 * File:    Ex_16_date_pack.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file demonstrates bit packing and unpacking of date values by
 *   compressing a day, month, and year into a single 16-bit unsigned
 *   short integer and then extracting them back. It uses bitwise
 *   operations to pack the date components into specific bit ranges (5
 *   bits for day, 4 bits for month, 7 bits for year) and corresponding
 *   masks to unpack them.
 */
//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//Typedefs
unsigned short pack_date(char day, char month, char year);
void unpack_date(unsigned short packed, int unpacked[3]);

//MAIN
int main(void){ //user input not proteced
    int day=0;
    int month=0;
    int year=0;
    printf("give the date: (dd MM YY):\n");
    scanf("%d%d%d", &day, &month, &year);
    printf("The date before packing is: %d/%d/%d\n", day, month, year);
    unsigned short pack = pack_date(day, month, year);
    int unpacked[3];
    unpack_date(pack, unpacked); 
    printf("The date after packing is: %d/%d/%d\n", unpacked[0], unpacked[1], unpacked[2]);

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


