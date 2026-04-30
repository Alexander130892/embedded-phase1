/*
 * File:    Ex_28_double_ended_queue.c
 * Author:  Alexander130892
 * Date:    31-3-2026
 *
 * Description:
 *   This file implements a double-ended queue (deque) data structure
 *   using a fixed-size array, allowing insertion and removal of
 *   elements from both the front and rear ends. The implementation
 *   uses a bidirectional approach starting from the middle of the
 *   array, expanding towards both ends, and includes comprehensive
 *   test cases demonstrating all operations including overflow and
 *   underflow conditions.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>


//Consts
#define MAX_LEN 1000
#define MIDDLE MAX_LEN/2
#define EMPTY   -1
#define FULL    (MAX_LEN-1)

//typedefs
typedef int data;
typedef struct{
    data data[MAX_LEN];
    int front;
    int rear;
}deque;
typedef enum {false, true} boolean;

//prototypes
void    reset(deque*);
void    add_to_front(deque *, data);
void    add_to_rear(deque *, data);
data    take_from_front(deque *);
data    take_from_rear(deque *);
boolean empty(deque*);
boolean full(deque*);

int main() {
    deque dq;
    reset(&dq);

    // Test adding to front and rear
    add_to_front(&dq, 10);
    add_to_rear(&dq, 20);
    add_to_front(&dq, 30);
    add_to_rear(&dq, 40);

    // Test taking from front and rear
    printf("Taken from front: %d\n", take_from_front(&dq)); // Should be 30
    printf("Taken from rear: %d\n", take_from_rear(&dq));   // Should be 40
    printf("Taken from front: %d\n", take_from_front(&dq)); // Should be 10
    printf("Taken from rear: %d\n", take_from_rear(&dq));   // Should be 20

    // Test empty deque
    printf("Taken from front: %d\n", take_from_front(&dq)); // Should print error
    printf("Taken from rear: %d\n", take_from_rear(&dq));   // Should print error

    // Test full deque
    for (int i = 0; i < MAX_LEN/2 ; i++) {
        add_to_front(&dq, i);
    }
    add_to_front(&dq, 100); // Should print error

    return EXIT_SUCCESS;
}

void    reset(deque* deq){
    deq->front=MIDDLE;
    deq->rear=MIDDLE;
}
void add_to_front(deque* deq, data d) {
    if (!full(deq)) {
        deq->data[++deq->front] = d;
    } else {
        printf("Deque is full, cannot add to front.\n");
    }
}

void add_to_rear(deque* deq, data d) {
    if (!full(deq)) {
        deq->data[--deq->rear] = d;
    } else {
        printf("Deque is full, cannot add to rear.\n");
    }
}
boolean full(deque* deq)
{
    return (deq->front == MAX_LEN - 1 && deq->rear == 0) || (deq->front == deq->rear - 1);
}
boolean empty(deque* deq)
{
    return (deq->front == MIDDLE && deq->rear == MIDDLE);
}
data take_from_front(deque* deq) {
    if (!empty(deq)) {
        return deq->data[deq->front--];
    } else {
        printf("Deque is empty, cannot take from front.\n");
        return EMPTY;
    }
}

data take_from_rear(deque* deq) {
    if (!empty(deq)) {
        return deq->data[deq->rear++];
    } else {
        printf("Deque is empty, cannot take from rear.\n");
        return EMPTY;
    }
}