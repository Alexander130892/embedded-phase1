/*
 * File:    Ex_15_doublelinkedlist.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This file implements a doubly-linked list data structure that
 *   stores characters, providing functions to create lists from
 *   strings, insert/delete nodes at various positions, and print the
 *   list in both forward and reverse directions. The main function
 *   demonstrates the functionality by converting "TestString" to a
 *   linked list, printing it both ways, deleting a character, and
 *   cleaning up memory.
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
struct DoubleLinkedList{
    DATA data;
    struct DoubleLinkedList* next;
    struct DoubleLinkedList* previous;
};
typedef struct DoubleLinkedList ELEMENT;
typedef ELEMENT*    LINK;

LINK string_to_list(char s[]);
void print_ll(LINK head);
void print_ll_reverse(LINK head);
void free_ll(LINK head);

void insert_at_head(LINK* head, DATA data);
void insert_at_tail(LINK head, DATA data);
void insert_after(LINK head, DATA search, DATA data);
void insert_at_n(LINK* head, int n, DATA data);
int delete_data(LINK* head, DATA data);


int main() {
    // Test data
    char *a = "TestString";
    LINK ll = string_to_list(a);
    print_ll(ll);
    print_ll_reverse(ll);
    // insert_at_head(&ll, 'A');
    // insert_at_tail(ll, 'A');
    // insert_after(ll, 's','A');
    // insert_at_n(&ll, 3,'B');
    // insert_at_n(&ll, 23,'B');
    delete_data(&ll, 'T');
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
    
    if(s[0] == '\0') return NULL;
    else{
        LINK head;
        head = malloc(sizeof(ELEMENT));
        head->data = s[0];
        head->previous=NULL;
        head->next = string_to_list(s+1);
        
        //return up the list to attach previous
        if(head->next != NULL)
            head->next->previous=head;
        return head;

    }
   
}
void print_ll(LINK head){
    if(head == NULL){
        printf("NULL\n");
        return;
    }
        
    LINK current = head;
    while(current != NULL){
        printf("%c --> ", current->data);
        if(current->next==NULL)
            printf("NULL\n");
        current=current->next;   
    }
}
void print_ll_reverse(LINK head){
    if(head == NULL){
        printf("NULL\n");
        return;
    }
        
    LINK current = head;
    while(current->next != NULL){
        current=current->next;   
    }
    printf("NULL <-- ");
    
    while(current != NULL){
        printf("%c <-- ", current->data);
        if(current->previous==NULL)
            printf("NULL\n");   
        current=current->previous;
    }
    
        
}
int delete_data(LINK* head, DATA data){
    if(*head == NULL) return 0;
    LINK current=*head;
    //search until end
    while(current != NULL && current->data != data){
        current=current->next;
    }
    if(current == NULL) // not found
        return 0;
    
    if (current->previous != NULL)
        current->previous->next = current->next;
    else
        *head = current->next;      // deleted node was head

    // fix next link
    if (current->next != NULL)
        current->next->previous = current->previous;
    // no else needed — tail->next is already NULL
    free(current);
    return 1;
}
void insert_at_head(LINK* head, DATA data){
    LINK new = malloc(sizeof(ELEMENT));
    new->data=data;
    new->next=*head;
    *head=new;
}
void insert_at_tail(LINK head, DATA data){
    if(head==NULL) return;
    LINK new = malloc(sizeof(ELEMENT));
    new->data=data;
    new->next=NULL;
    LINK tail=head;
    while(tail->next != NULL){
        tail=tail->next;
    }
    tail->next=new;
}
void insert_after(LINK head, DATA search, DATA data){
    
    if(head == NULL) return;
    LINK current=head;
    //search until end
    while(current != NULL && current->data != search){
        current=current->next;
    }
    if(current == NULL) // not found
        return;
    LINK new = malloc(sizeof(ELEMENT));
    new->data=data;
    new->next=current->next;
    current->next=new;
    return;
}
void insert_at_n(LINK* head, int n, DATA data){
    LINK new = malloc(sizeof(ELEMENT));
    new->data=data;
    if(n==0 || *head == NULL){
        new->next=*head;
        *head=new;
        return;
    }
    int count = 0;
    LINK current=*head;
    while(current->next != NULL && count < n-1){
        current=current->next;
        count++;
    }
    new->next=current->next;
    current->next=new;
    return;
}