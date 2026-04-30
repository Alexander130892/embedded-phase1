/*
 * File:    Ex_03_04_list_sorted_DATA.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This C file implements a linked list data structure for storing
 *   person records (name, age, weight) with functionality to create,
 *   print, sort, and perform queries on the list. It demonstrates
 *   various linked list operations including recursive creation and
 *   memory management, bubble sort with customizable comparison
 *   functions, and counting records that meet specific criteria.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>


//Consts
typedef struct {
    char name[10];
    int age;
    int weight;
} DATA;
struct LinkedList{
    DATA data;
    struct LinkedList* next;
};
typedef struct LinkedList ELEMENT;
typedef ELEMENT*    LINK;

LINK create_ll(DATA* data_array, int size);
void print_ll(LINK head);
void free_ll(LINK head);
void sort_ll(LINK head, int (*compare)(const DATA*, const DATA*));

int above_weight_age(LINK head, int age_limit, int  weight_limit);
int compare_age(const DATA * age1, const DATA * age2 );
int compare_weight(const DATA * w1, const DATA * w2 );
int compare_name(const DATA * name1, const DATA * name2 );


int main() {
    // Test data
    DATA people[] = {
        {"Alice",  30, 65},
        {"Bob",    25, 80},
        {"Carol",  35, 70},
        {"Dave",   28, 90},
        {"Eve",    22, 55}
    };
    int size = sizeof(people) / sizeof(people[0]);
    printf("=== Test 1: create and print ===\n");
    LINK list = create_ll(people, size);
    print_ll(list);
    // Test 2 — single element
    printf("\n=== Test 2: single element ===\n");
    LINK single = create_ll(people, 1);
    print_ll(single);

    // Test 3 — empty list
    printf("\n=== Test 3: empty (expect NULL) ===\n");
    LINK empty = create_ll(people, 0);
    printf("Result: %s\n", empty == NULL ? "NULL" : "NOT NULL");

    // Test 4 — verify links
    printf("\n=== Test 4: manual traversal ===\n");
    LINK current = list;
    int index = 0;
    while (current != NULL) {
        printf("Node %d: %s, age %d, weight %d — next: %s\n",
               index++,
               current->data.name,
               current->data.age,
               current->data.weight,
               current->next ? current->next->data.name : "NULL");
        current = current->next;
    }
    // Test 5 - above certain weight and age
    printf("\n=== Test 5: above certain weight and age ===\n");
    printf("%d are above age: %d and weight: %d  (Expect 3)\n", above_weight_age(list, 25,70), 25, 70);
    // cleanup
    printf("\n=== Test 6: Sort by age ===\n");
    sort_ll(list, compare_age);   // sort by age
    print_ll(list);
    printf("\n=== Test 7: Sort by name ===\n");
    sort_ll(list, compare_name);  // sort by name
    print_ll(list);
    printf("\n=== Test 8: Sort by weight ===\n");
    sort_ll(list, compare_weight);  // sort by name
    print_ll(list);
    free_ll(list);
    free_ll(single);
    return EXIT_SUCCESS;
}

LINK create_ll(DATA* data_array, int size)
{
    if(size<=0)
        return NULL;
    else{
        LINK head = (LINK) malloc(sizeof(ELEMENT));
        head->data=data_array[0];
        head->next=create_ll(data_array+1,size-1);
        return head;
    }
}
void print_ll(LINK head) {
    LINK current = head;
    while (current != NULL) {
        printf("Name: %-10s Age: %-4d Weight: %d\n",
               current->data.name,
               current->data.age,
               current->data.weight);
        current = current->next;
    }
}
void free_ll(LINK head)
{
    if(head==NULL) return;
    free_ll(head->next);
    free(head);
}
int above_weight_age(LINK head, int age_limit, int  weight_limit)
{
    int count = 0;
    LINK current = head;
    while(current != NULL)
    {
        if((current->data.age >= age_limit) && (current->data.weight >= weight_limit)) count++;
        current = current->next;
    }
    return count;
}
int compare_age(const DATA * age1, const DATA * age2 ){
    return age1->age - age2->age;
}
int compare_name(const DATA * name1, const DATA * name2 ){
    return strcmp(name1->name,name2->name);
}
int compare_weight(const DATA * w1, const DATA * w2 ){
    return w1->weight - w2->weight;
}
void sort_ll(LINK head, int (*compare)(const DATA*, const DATA*)){
    if(head == NULL) return;
    int swapped;
    do {
        swapped = 0;
        LINK current = head;
        while (current->next != NULL) {
            if (compare(&current->data, &current->next->data) > 0) {
                DATA temp           = current->data;
                current->data       = current->next->data;
                current->next->data = temp;
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped);
}
