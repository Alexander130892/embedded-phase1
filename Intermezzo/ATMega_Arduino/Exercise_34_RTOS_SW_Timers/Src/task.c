/*
 * File:    task.c
 * Author:  Alexander130892
 * Date:    8-6-2026
 *
 * Description:
 *   This file implements task functions for an AVR microcontroller
 *   system that manage hardware peripherals including three LEDs (red,
 *   green, blue), a buzzer, buttons, and UART communication. Each task
 *   function handles a specific peripheral operation—toggling LEDs,
 *   activating the buzzer, detecting button presses, and sending debug
 *   tick messages over UART.
 */



#include <stdint.h>
#include <avr/io.h>
#include "task.h"
#include "timer.h"
#include "hw_uart.h"
#include "gpio.h"
#include <stdbool.h>

volatile bool b_button_pressed = false;

void taskLed_R(void){
    LED_PORT ^=  (1 << LED_R_PIN);   /* Toggle LED */
}
void taskLed_G(void){
    LED_PORT ^=  (1 << LED_G_PIN);   /* Toggle LED */
}
void taskLed_B(void){
    LED_PORT ^=  (1 << LED_B_PIN);   /* Toggle LED */
}
void taskBuzzer(void) {
    TCCR1A |= (1 << COM1A1);        // connect OC1A → buzzer on
    sw_timerStart(&timer_buzzer_off, 500);
}
void taskButtons(void){
    if((PIND & (1u << PB_PIN))){
        b_button_pressed = true;
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

