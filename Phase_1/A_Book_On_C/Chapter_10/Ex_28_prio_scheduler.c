/*
 * File:    Ex_28_prio_scheduler.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This file implements a multi-level priority queue scheduler that
 *   reads job requests labeled 'A', 'B', 'C', or 'D' from input,
 *   stores them in separate priority queues with A having highest
 *   priority (1) and D lowest (4), then processes and displays all
 *   jobs from each queue in priority order.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>

#define EMPTY   0
#define FULL    10000

typedef unsigned int    DATA;
typedef enum{false, true} boolean;
typedef struct element{
    DATA            data;
    int             priority; // 1 , 2, 3 (1 is highest prio)
    struct element* next;
}ELEMENT;
typedef struct queue{
    int cnt;
    ELEMENT* front;
    ELEMENT* rear;
} QUEUE;

void initialize(QUEUE *q);
void enqueue(QUEUE *q, DATA d, int prio);
DATA dequeue(QUEUE *q);
DATA front(const QUEUE* q);
boolean isempty(const QUEUE *q);
boolean isfull(const QUEUE *q);


int main(){
    int ch;
    int cnt_a = 0;
    int cnt_b = 0;
    int cnt_c = 0;
    int cnt_d = 0;
    DATA pid;
    QUEUE a,b,c,d;

    initialize(&a);
    initialize(&b);
    initialize(&c);
    initialize(&d);
    //Enqueue the requests
    while((ch=getchar()) != EOF){
        switch(ch){
            // using queue letter as priority: A=1, B=2, C=3, D=4
            case 'A':
                assert(scanf("%u", &pid) == 1);
                if (!isfull(&a))
                    enqueue(&a, pid, 1);
                break;
            case 'B':
                assert(scanf("%u", &pid) == 1);
                if (!isfull(&b))
                    enqueue(&b, pid, 2);
                break;
            case 'C':
                assert(scanf("%u", &pid) == 1);
                if (!isfull(&c))
                    enqueue(&c, pid, 3);
                break;
            case 'D':
                assert(scanf("%u", &pid) == 1);
                if (!isfull(&d))
                    enqueue(&d, pid, 4);
                break;
        }
    }
    //Dequeue the requests and print
    printf("\nA's schedule:\n");
    while(!isempty(&a)){
        pid = dequeue(&a);
        printf("    JOB %u is %d\n", ++cnt_a, pid);
    }
    printf("\nB's schedule:\n");
    while(!isempty(&b)){
        pid = dequeue(&b);
        printf("    JOB %u is %d\n", ++cnt_b, pid);
    }
    printf("\nC's schedule:\n");
    while(!isempty(&c)){
        pid = dequeue(&c);
        printf("    JOB %u is %d\n", ++cnt_c, pid);
    }
    printf("\nD's schedule:\n");
    while(!isempty(&d)){
        pid = dequeue(&d);
        printf("    JOB %u is %d\n", ++cnt_d, pid);
    }
    return EXIT_SUCCESS;
}

void initialize(QUEUE *q){
    q->cnt=0;
    q->front=NULL;
    q->rear=NULL;
}
void enqueue(QUEUE *q, DATA d, int prio) { //Priority Queue --> First item is always highest prio
    if (isfull(q)) return;

    ELEMENT* p    = malloc(sizeof(ELEMENT));
    p->data       = d;
    p->priority   = prio;
    p->next       = NULL;

    // insert at head if queue empty or new item has higher/equal priority
    if (isempty(q) || prio <= q->front->priority) {
        p->next  = q->front;
        q->front = p;
        if (q->cnt == 0)
            q->rear = p;
    } else {
        // find insertion point
        ELEMENT* current = q->front;
        while (current->next != NULL && current->next->priority <= prio)
            current = current->next;
        p->next      = current->next;
        current->next = p;
        if (p->next == NULL)
            q->rear = p;
    }
    q->cnt++;
}
DATA dequeue(QUEUE *q){
    if(isempty(q)) return 0;  // guard against empty
    DATA d;
    ELEMENT *p;
    d        = q->front->data;
    p        = q->front;
    q->front = q->front->next;
    if(q->front == NULL)
        q->rear = NULL;        //  keep rear in sync
    q->cnt--;
    free(p);
    return d;
}
DATA front(const QUEUE* q){
    return q->front->data;
}
boolean isempty(const QUEUE *q){
    return (boolean) (q->cnt == EMPTY);
}
boolean isfull(const QUEUE *q){
    return (boolean) (q->cnt == FULL);
}