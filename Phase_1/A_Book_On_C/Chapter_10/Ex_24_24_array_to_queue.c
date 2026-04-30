/*
 * File:    Ex_24_24_array_to_queue.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This C program implements a linked list-based queue data structure
 *   and demonstrates converting an array to a queue and then back to
 *   an array, effectively preserving the original order through FIFO
 *   operations. The main function creates an array, converts it to a
 *   queue using enqueue operations, then converts it back to an array
 *   using dequeue operations and prints the result.
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
    struct element* next;
}ELEMENT;
typedef struct queue{
    int cnt;
    ELEMENT* front;
    ELEMENT* rear;
} QUEUE;

void initialize(QUEUE *q);
QUEUE data_to_queue(DATA array[], int n);
DATA* queue_to_data(QUEUE q);
void enqueue(QUEUE *q, DATA d);
DATA dequeue(QUEUE *q);
DATA front(const QUEUE* q);
boolean isempty(const QUEUE *q);
boolean isfull(const QUEUE *q);


int main(){
    DATA array[10]={9,8,7,6,5,4,3,2,1,9};
    QUEUE q=data_to_queue(array, 10);
    DATA* ret=queue_to_data(q);
    for(int i=0; i<10;i++){
        printf("%d ", ret[i]);
    }
    putchar('\n');
    return EXIT_SUCCESS;
}

void initialize(QUEUE *q){
    q->cnt=0;
    q->front=NULL;
    q->rear=NULL;
}
QUEUE data_to_queue(DATA array[], int n){
    QUEUE q;
    initialize(&q);
    for(int i=0; i<n;i++){
        enqueue(&q,array[i]);
    }
    return q;
}
DATA* queue_to_data(QUEUE q){
    int n = q.cnt;
    DATA *array=malloc(sizeof(DATA)*n);
    for(int i=0; i<n;i++){
        array[i]=dequeue(&q);
    }
    return array;
}
void enqueue(QUEUE *q, DATA d){
    ELEMENT * p = malloc(sizeof(ELEMENT));
    p->data = d;
    p->next = NULL;
    if(!isempty(q)){
        q->rear->next=p;
        q->rear = p;
    }else{
        q->front = q ->rear = p;
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