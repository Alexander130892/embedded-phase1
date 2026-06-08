

#include <stdint.h>
#include <avr/io.h>
#include "task.h"
#include "gpio.h"

void taskLed_R(void){
    LED_PORT ^=  (1 << LED_R_PIN);   /* Toggle LED */
}
void taskLed_G(void){
    LED_PORT ^=  (1 << LED_G_PIN);   /* Toggle LED */
}
void taskLed_B(void){
    LED_PORT ^=  (1 << LED_B_PIN);   /* Toggle LED */
}
void taskButtons(void){
    if((PIND & (1u << PB_PIN))){
        hw_uart_queue('P');
        hw_uart_queue('B');
        hw_uart_queue('\r');
        hw_uart_queue('\n');
    }
}
void taskTick(void){
    hw_uart_queue('T');
    hw_uart_queue('i');
    hw_uart_queue('c');
    hw_uart_queue('k');
    hw_uart_queue('\r');
    hw_uart_queue('\n');
}
void taskSerial(void){
    hw_uart_drain();
}

