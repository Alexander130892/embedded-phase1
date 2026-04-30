/*
 * File:    Ex_11_insert_head.c
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This file implements a linked list data structure that stores
 *   characters, with functionality to convert strings to linked lists
 *   and insert new elements at the head of the list. The program
 *   demonstrates creating a linked list from the string "TestString",
 *   inserting the character 'A' at the beginning, and displaying the
 *   results.
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

void insert_at_head(LINK* head, DATA data);


int main() {
    // Test data
    char *a = "TestString";
    LINK ll = string_to_list(a);
    print_ll(ll);
    insert_at_head(&ll, 'A');
    print_ll(ll);
    free_ll(ll);
    return EXIT_SUCCESS;
}

void free_ll(LINK head)
{
    if(head==NULL) return;
    free_ll(head->next);
    free(head);
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
void insert_at_head(LINK* head, DATA data){
    LINK new = malloc(sizeof(ELEMENT));
    new->data=data;
    new->next=*head;
    *head=new;
}