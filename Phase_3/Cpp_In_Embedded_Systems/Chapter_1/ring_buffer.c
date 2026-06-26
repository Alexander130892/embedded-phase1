#include <stdio.h>

#define BUFFER_SIZE 5

typedef struct {
    int arr[BUFFER_SIZE];
    size_t write_idx;
    size_t read_idx;
    size_t count;
} int_ring_buffer;

void int_ring_buffer_init(int_ring_buffer *irb){
    irb->write_idx=0;
    irb->read_idx=0;
    irb->count=0;
}

void int_ring_buffer_push(int_ring_buffer *irb, int data){
    irb->arr[irb->write_idx] = data;
    irb->write_idx = (irb->write_idx + 1) % BUFFER_SIZE;
    if(irb->count < BUFFER_SIZE){
        irb->count++;
    }else{
        //Buffer full -> overwrite
        irb->read_idx = (irb->read_idx + 1) % BUFFER_SIZE;
    }
}

int int_ring_buffer_pop(int_ring_buffer *irb){
    if(irb->count == 0){
        return 0;
    }else{
        int data = irb->arr[irb->read_idx];
        irb->read_idx = (irb->read_idx + 1) % BUFFER_SIZE;
        irb->count--;
        return data;
    }
}


int main() {
    int_ring_buffer irb;
    int_ring_buffer_init(&irb);

    for (int i = 0; i < 10; i++){
        int_ring_buffer_push(&irb, i);
    }

    while(irb.count > 0){
        printf("%d ", int_ring_buffer_pop(&irb));
    }
    printf("\r\n");
    return 0;
}