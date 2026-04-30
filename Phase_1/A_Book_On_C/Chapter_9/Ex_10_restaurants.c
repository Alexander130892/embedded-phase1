/*
 * File:    Ex_10_restaurants.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This program manages a database of 20 restaurants with different
 *   food types and costs, then filters and displays all hamburger
 *   restaurants sorted by their average cost in ascending order. The
 *   program uses a struct to store restaurant data and includes a
 *   comparison function for sorting the entire list before filtering
 *   by the specified food type.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

//Consts
#define AMOUNT 20 //rows

enum food_type{PIZZA, ITALIAN, PITA, HAMBURGER, PASTA, MEXICAN, MAX_TYPES};
const char* food_type_str[]={"PIZZA", "ITALIAN", "PITA", "HAMBURGER", "PASTA", "MEXICAN"};

typedef struct{
    char*           name;
    enum food_type  type;
    double          average_cost;
} restaurant;

void print_restos_type(restaurant* list, enum food_type type);
int compare_cost(const void* a, const void* b) {
    restaurant* ra = (restaurant*)a;
    restaurant* rb = (restaurant*)b;
    if (ra->average_cost < rb->average_cost) return -1;
    if (ra->average_cost > rb->average_cost) return  1;
    return 0;
}

int main(){
    //init resto lists
    restaurant list[AMOUNT] = {
    {"Napoli Express",    PIZZA,     12.50},
    {"Roma Centrale",     ITALIAN,   18.00},
    {"Pita Palace",       PITA,       9.75},
    {"Burger Barn",       HAMBURGER, 11.00},
    {"Pasta Pronto",      PASTA,     14.50},
    {"El Rancho",         MEXICAN,   13.00},
    {"Pizza Volcano",     PIZZA,      8.99},
    {"Trattoria Bella",   ITALIAN,   22.50},
    {"Shawarma King",     PITA,      10.50},
    {"Big Smoke Burger",  HAMBURGER, 15.00},
    {"La Carbonara",      PASTA,     17.00},
    {"Taco Loco",         MEXICAN,    9.50},
    {"Margherita House",  PIZZA,     16.00},
    {"Osteria del Porto", ITALIAN,   25.00},
    {"Falafel Corner",    PITA,       7.50},
    {"Smash & Stack",     HAMBURGER, 13.75},
    {"Penne & Co",        PASTA,     11.50},
    {"Casa Mexicana",     MEXICAN,   14.00},
    {"Diavola Pizza",     PIZZA,     19.00},
    {"Burger Republic",   HAMBURGER, 12.00}
    };
    print_restos_type(list, HAMBURGER);

    return EXIT_SUCCESS;
}

void print_restos_type(restaurant* list, enum food_type type){
    qsort(list, AMOUNT, sizeof(restaurant), compare_cost);
    printf("The restuarants for type %s, ordered by cost:\n", food_type_str[type]);
    for(int i = 0; i<AMOUNT ; i++)
    {
        
        if(list[i].type == type )
            printf("Name: %s - Average cost: %.2f\n", list[i].name, list[i].average_cost);
    }
}