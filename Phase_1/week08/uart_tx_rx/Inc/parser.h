/*
 * File:    parser.h
 * Author:  Alexander130892
 * Date:    30-4-2026
 *
 * Description:
 *   This header file defines a command parser system that maps command
 *   names to handler functions, designed to process text-based
 *   commands (likely from UART communication) and execute
 *   corresponding actions like LED control, PWM control, and status
 *   reporting.
 */
#ifndef PARSER_H
#define PARSER_H

#define BUFFERSIZE 64


typedef void (*command_handler_t)(const char *arg);

typedef struct {
    const char *name;
    command_handler_t handler;
} command_t;

//temporary function to simulate UART communication
//Real function to be implemented at later stage
void handle_led(const char *arg);
void handle_pwm(const char *arg);
void handle_status(const char *arg);
void parse_command(const char* input);

#endif // PARSER_H




