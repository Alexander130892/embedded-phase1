/*
 * File:    parser.c
 * Author:  Alexander130892
 * Date:    29-4-2026
 *
 * Description:
 *   This file implements a command parser that processes text commands
 *   ("led", "pwm", "status") and dispatches them to appropriate
 *   handler functions for controlling hardware peripherals. The
 *   handlers currently output test messages to simulate UART
 *   communication, with the LED handler accepting on/off arguments,
 *   PWM handler validating numeric values 0-100, and status handler
 *   taking no arguments.
 */

#include "stm32f446re.h"
#include "timer.h"
#include "parser.h"
#include "uart.h"
#include "led.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static const command_t commands[] = {
    { "led",    handle_led    },
    { "pwm",    handle_pwm    },
    { "status", handle_status },
};

void handle_led(const char *arg){
     if(strcmp(arg, "on") == 0){
		uart_send_string("The LED is ON! \r\n");
		led_on();
	 }else if(strcmp(arg, "off") == 0){
		uart_send_string("the LED is OFF! \r\n");
		led_off();
	 }else{
		uart_send_string("Incorrect argument for LED!\r\n");
	 }
}
void handle_pwm(const char *arg){
    char *end;
    int32_t arg_pwm = strtol(arg, &end, 10);
    if (end == arg || *end != '\0') {
    	uart_send_string("Incorrect argument for PWM - Not a integer in [0-100]!\r\n");
    return;
    }
    if(arg_pwm < 0 || arg_pwm > 100){
    	uart_send_string("Incorrect argument for PWM - Not a integer in [0-100]!\r\n");
    }else{
    	char output[40];
    	g_duty_cycle = (uint32_t)arg_pwm;
    	g_CCR = g_duty_cycle * (ARR + 1) / 100;
    	// Set TIMx_CCR1 (initial duty cycle) D = CCR1 / (ARR+1)
    	*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_CCR1_OFFSET))		= g_CCR;
    	sprintf(output, "The LED has been set to a PWM of %d\r\n", arg_pwm);
    	uart_send_string(output);
    }    
}
void handle_status(const char *arg){
    if (strlen(arg) > 0)
    	uart_send_string("Warning - Status requires no argument!\r\n");
    bool state = led_get_state();
    char output[BUFFERSIZE];
    sprintf(output, "%s%d%s%d%s", "LED State: ", state, " PWM: ", g_duty_cycle ,"\r\n");
    uart_send_string(output);
}

void parse_command(const char* input){
    char command[BUFFERSIZE];
    char argument[BUFFERSIZE] = "";
    // Parse the input into command and optional argument
    if (sscanf(input, "%63s %63[^\n]", command, argument) < 1) {
    	uart_send_string("Invalid input!\r\n");
        return;
    }
    for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strcmp(command, commands[i].name) == 0) {
            commands[i].handler(argument);
            return;
        }
    }
    uart_send_string("Invalid input!\r\n");
}
