/*
 * File:    main.c
 * Author:  Alexander130892
 * Date:    1-5-2026
 *
 * Description:
 *   This file simulates a UART command interface by reading a command
 *   string from stdin, parsing it into a command and optional argument
 *   (like "led on" or "pwm 75"), and dispatching it to appropriate
 *   handlers using a function pointer table pattern commonly used in
 *   embedded systems.
 */
// Read a command string (from stdin to simulate UART input)
// Parse it into a command + optional argument: led on, led off, pwm 75, status
// Dispatch to a handler using a function pointer table (this is the embedded pattern — a table of {command_string, handler_fn} structs)
// Print a response to stdout


#include <stdio.h>
#include "parser.h"


int main(void){
    // Read command string from std in
    char input[BUFFERSIZE];
    
    

    fprintf(stdout, "Test - simulate input:\n");

    // Read the entire line using fgets
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    parse_command(input);
    return 0;
}