/*
 * File:    fixed_array_stack.c
 * Author:  Alexander130892
 * Date:    3-4-2026
 *
 * Description:
 *   This file implements a stack data structure using a fixed-size
 *   array with a maximum capacity of 100 integer elements, providing
 *   standard stack operations (push, pop, top, isEmpty, isFull). The
 *   main function contains comprehensive tests that demonstrate the
 *   stack's LIFO behavior, boundary conditions, and error handling for
 *   empty and full stack scenarios.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//Consts
#define MAX_COUNT 100
#define EMPTY -1
//Macros

//Typedefs
typedef int data;
typedef enum{false, true} boolean;
typedef struct{
    int cnt;
    data array[MAX_COUNT];
}stack;


//Prototype Functions
data top(stack* p_stack);
data pop(stack* p_stack);
void push(stack* p_stack, data d);
boolean isempty(stack* p_stack);
boolean isfull(stack* p_stack);

//MAIN 
int main(void) {
    stack s = {0};

    // Test 1 — empty check
    printf("=== Test 1: initial state ===\n");
    printf("Empty (expect 1): %d\n", isempty(&s));
    printf("Full  (expect 0): %d\n", isfull(&s));

    // Test 2 — push and top
    printf("\n=== Test 2: push + top ===\n");
    push(&s, 10);
    push(&s, 20);
    push(&s, 30);
    printf("Top (expect 30): %d\n", top(&s));
    printf("Empty (expect 0): %d\n", isempty(&s));

    // Test 3 — pop order (LIFO)
    printf("\n=== Test 3: pop order (expect 30, 20, 10) ===\n");
    printf("Pop: %d\n", pop(&s));
    printf("Pop: %d\n", pop(&s));
    printf("Pop: %d\n", pop(&s));
    printf("Empty (expect 1): %d\n", isempty(&s));

    // Test 4 — pop from empty
    printf("\n=== Test 4: pop from empty (expect -1) ===\n");
    printf("Pop: %d\n", pop(&s));

    // Test 5 — fill to full
    printf("\n=== Test 5: fill to full ===\n");
    for (int i = 0; i < MAX_COUNT; i++)
        push(&s, i);
    printf("Full (expect 1): %d\n", isfull(&s));
    printf("Top (expect 99): %d\n", top(&s));

    // Test 6 — push onto full stack
    printf("\n=== Test 6: push onto full stack ===\n");
    push(&s, 999);  // should print "Stack is full!"
    printf("Top still (expect 99): %d\n", top(&s));

    // Test 7 — partial drain
    printf("\n=== Test 7: partial drain ===\n");
    for (int i = 0; i < 5; i++)
        printf("Pop: %d\n", pop(&s));
    printf("Full (expect 0): %d\n", isfull(&s));
    printf("Empty (expect 0): %d\n", isempty(&s));

    return EXIT_SUCCESS;
}

data top(stack* p_stack){
    return p_stack->array[p_stack->cnt-1];
}
data pop(stack* p_stack){
    if (!isempty(p_stack))
    {
        return p_stack->array[--p_stack->cnt];
    }else return EMPTY;
}
void push(stack* p_stack, data d){
    if (!isfull(p_stack))
    {
        p_stack->array[p_stack->cnt++]=d;
    }else printf("Stack is full!\n");
}
boolean isempty(stack* p_stack){
    if(p_stack->cnt == 0)
        return true;
    else 
        return false;
}
boolean isfull(stack* p_stack){
    if(p_stack->cnt == MAX_COUNT)
        return true;
    else 
        return false;
}