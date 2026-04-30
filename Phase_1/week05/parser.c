
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static const command_t commands[] = {
    { "led",    handle_led    },
    { "pwm",    handle_pwm    },
    { "status", handle_status },
};

//temporary function to simulate UART comminucation
//Real function to be implemented at later stage
void handle_led(const char *arg){
     if(strcmp(arg, "on") == 0){
            fprintf(stdout, "I test the LED function! %s\n", arg);
        }else if(strcmp(arg, "off") == 0){
            fprintf(stdout, "I test the LED function! %s\n", arg);
        }else{
            fprintf(stderr, "Invalid argument for led!\n");
        }
}
void handle_pwm(const char *arg){
    char *end;
    int32_t arg_pwm = strtol(arg, &end, 10);
    if (end == arg || *end != '\0') {
    fprintf(stderr, "Invalid argument for pwm! Not a number.\n");
    return;
    }
    if(arg_pwm < 0 || arg_pwm > 100){
        fprintf(stderr, "Invalid argument for pwm! Should be integer in [0-100]\n");
    }else{
        fprintf(stdout, "I test the PWM function! %d\n", arg_pwm);
    }    
}
void handle_status(const char *arg){
    if (strlen(arg) > 0)
        fprintf(stderr, "Warning: status takes no argument\n");
    fprintf(stdout, "I test the Status function! %s\n", arg);
}

void parse_command(const char* input){
    char command[BUFFERSIZE];
    char argument[BUFFERSIZE] = "";
    // Parse the input into command and optional argument
    if (sscanf(input, "%19s %19[^\n]", command, argument) < 1) {
        fprintf(stderr, "Invalid input format.\n");
        return;
    }
    for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strcmp(command, commands[i].name) == 0) {
            commands[i].handler(argument);
            return;
        }
    }
    fprintf(stderr, "Incorrect command!\n");
}