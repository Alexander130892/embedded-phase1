/*
 * File:    Ex_09_concat_extra.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This file implements a linked list library for character data,
 *   providing functions to create lists from strings, concatenate two
 *   lists, clone lists, and detect cycles. The main function
 *   demonstrates converting two strings to linked lists, concatenating
 *   them, and testing for cycles before freeing the memory.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>


//Consts
typedef char DATA;
struct LinkedList{
    DATA data;
    struct LinkedList* next;
};
typedef struct LinkedList ELEMENT;
typedef ELEMENT*    LINK;

LINK create_ll(DATA* data_array, int size);
LINK string_to_list(char s[]);
void print_ll(LINK head);
void free_ll(LINK head);
void sort_ll(LINK head, int (*compare)(const DATA*, const DATA*));
int count(LINK head);
LINK concatenate(LINK a, LINK b);
LINK clone_ll(LINK a);

int above_weight_age(LINK head, int age_limit, int  weight_limit);
int compare_age(const DATA * age1, const DATA * age2 );
int compare_weight(const DATA * w1, const DATA * w2 );
int compare_name(const DATA * name1, const DATA * name2 );
int is_cycle(LINK head);


int main() {
    // Test data
    char *a = "TestString";
    char *b = "2ndString";
    LINK ll = string_to_list(a);
    print_ll(ll);
    LINK llb = string_to_list(b);
    print_ll(llb);
    printf("b: %d\n",is_cycle(llb));
    LINK c = concatenate(ll, llb);
    print_ll(c); 
    free_ll(ll);   
    free_ll(llb);
    free_ll(c);
    return EXIT_SUCCESS;
}

int is_cycle(LINK head){
    LINK initial_addr = head;
    if(head == NULL) return 0;
    LINK current=head;
    int count=0;
    do
    {
        if (current->next == NULL) return 0;
        if (current->next == initial_addr) return 1;
        current=current->next;
    } while (1);
}
void free_ll(LINK head)
{
    if(head==NULL) return;
    free_ll(head->next);
    free(head);
}
LINK clone_ll(LINK a){
    if(a == NULL) return NULL;
    LINK c = malloc(sizeof(ELEMENT));
    c->data = a->data;
    c->next = clone_ll(a->next);
    return c;
}
LINK string_to_list(char s[]){
    LINK head;
    if(s[0] == '\0') return NULL;
    else{
        head = malloc(sizeof(ELEMENT));
        head->data = s[0];
        head->next = string_to_list(s+1);
        return head;
    }
}
LINK concatenate(LINK a, LINK b){
    if(a==NULL && b == NULL) return NULL;
    if(a==NULL) return clone_ll(b);
    if(b==NULL) return clone_ll(a);

    LINK c = clone_ll(a);
    LINK tail=c;
    while(tail->next!=NULL){
        tail=tail->next;
    }
    tail->next=clone_ll(b);
    return c;
}
int count(LINK head){
    if(head == NULL)
        return 0;
    else
        return (1 + count(head->next));
}
void print_ll(LINK head){
    if(head == NULL)
        printf("NULL\n");
    LINK current = head;
    while(current != NULL){
        printf("%c --> ", current->data);
        if(current->next==NULL)
            printf("NULL\n");
        current=current->next;   
    }
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
