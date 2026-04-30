/*
 * File:    Ex_11.c
 * Author:  Alexander130892
 * Date:    25-3-2026
 *
 * Description:
 *   This program prompts the user to input their name and age, then
 *   displays a greeting message telling them what their age will be
 *   next year.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void){
    char name[20];
    int age;
    printf("Give me your name: ");
    scanf("%s", name);
    printf("\nand age: ");
    scanf("%d", &age);
    printf("\nHello %s, next year you will be %d\n", name, age+1);
    return EXIT_SUCCESS;
}