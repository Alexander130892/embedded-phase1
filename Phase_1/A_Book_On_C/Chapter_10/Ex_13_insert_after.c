/*
 * File:    Ex_13_insert_after.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This C program implements a linked list that stores character data
 *   and provides insertion operations including inserting at the head,
 *   tail, and after a specified character. It demonstrates these
 *   operations by converting the string "TestString" to a linked list
 *   and then inserting the character 'A' at various positions.
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
void insert_at_tail(LINK head, DATA data);
void insert_after(LINK head, DATA search, DATA data);


int main() {
    // Test data
    char *a = "TestString";
    LINK ll = string_to_list(a);
    print_ll(ll);
    insert_at_head(&ll, 'A');
    insert_at_tail(ll, 'A');
    insert_after(ll, 's','A');
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