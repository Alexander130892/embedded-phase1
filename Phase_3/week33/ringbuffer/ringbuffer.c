/*
 * File:    ringbuffer.c
 * Author:  Alexander130892
 * Date:    30-8-2026
 *
 * Description:
 *   This file implements a fixed-size circular buffer that stores 16
 *   uint32_t values, automatically overwriting the oldest data when
 *   full. It provides push/pop operations, capacity checks, and
 *   maintains read/write indices to manage the ring structure.
 */
#include <stdint.h>
#include <stdbool.h>

#define RINGBUFFERSIZE 16

typedef struct{
    uint8_t rd_idx;
    uint8_t wrt_idx;
    uint8_t count;
    uint32_t data[RINGBUFFERSIZE];
}ringbuffer_t;

// ringbuffer_t rb = {0};   // zeroes the whole struct

void rb_push(ringbuffer_t* rb, uint32_t val){
    rb->data[rb->wrt_idx] = val;
    rb->wrt_idx = (rb->wrt_idx + 1) % RINGBUFFERSIZE;
    if (rb->count < RINGBUFFERSIZE){
        rb->count++;
    }else{
        rb->rd_idx = (rb->rd_idx + 1) % RINGBUFFERSIZE;
    }
}

bool rb_pop(ringbuffer_t* rb, uint32_t* out){
    if(rb->count == 0){
        return false;
    }
    *out = rb->data[rb->rd_idx];
    rb->rd_idx = (rb->rd_idx + 1) % RINGBUFFERSIZE;
    rb->count--;
    return true;
}

bool is_empty(ringbuffer_t* rb){
    return (rb->count == 0);
}

uint8_t get_count(ringbuffer_t* rb){
    return rb->count;
}

