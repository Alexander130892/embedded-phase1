/*
 * File:    parser.c
 * Author:  Alexander130892
 * Date:    22-4-2026
 *
 * Description:
 *   This file implements a command parser that processes text commands
 *   received via UART to control LED and PWM functions on an embedded
 *   system. It parses commands like "led on/off", "pwm [0-100]", and
 *   "status", validates their arguments, and executes the
 *   corresponding handler functions while sending feedback messages
 *   over UART.
 */
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

//temporary function to simulate UART comminucation
//Real function to be implemented at later stage
void handle_led(const char *arg){
     if(strcmp(arg, "on") == 0){
    	 	uart_send_string("I test the LED function ON! \r\n");
    	 	led_on();
        }else if(strcmp(arg, "off") == 0){
        	uart_send_string("I test the LED function OFF! \r\n");
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
    	uart_send_string("I test the PWM function!\r\n");
    }    
}
void handle_status(const char *arg){
    if (strlen(arg) > 0)
    	uart_send_string("Warning - Status requires no argument!\r\n");
    bool state = led_get_state();
    char output[BUFFERSIZE];
    sprintf(output, "%s%d%s", "I test the status function! State: ", state, "\r\n");
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
