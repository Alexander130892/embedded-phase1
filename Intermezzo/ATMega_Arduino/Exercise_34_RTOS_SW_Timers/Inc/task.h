/*
 * File:    task.h
 * Author:  Alexander130892
 * Date:    7-6-2026
 *

 */

#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*TaskFunction)(void);
typedef struct {
    TaskFunction function;
    uint32_t period;
    uint32_t lastRun;
} Task;

extern volatile bool b_button_pressed;

void taskLed_R(void);
void taskLed_G(void);
void taskLed_B(void);
void taskBuzzer(void);
void taskButtons(void);
void taskTick(void);

#endif