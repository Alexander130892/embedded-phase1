/*
 * File:    task.h
 * Author:  Alexander130892
 * Date:    8-6-2026
 *
 * Description:
 *   This header file defines a task scheduling system with structures
 *   and function pointers for managing periodic tasks, while also
 *   declaring task functions for controlling LEDs (red, green, blue),
 *   a buzzer, buttons, and a system tick, along with a volatile flag
 *   for button state.
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

void schedulerRun(void);

#endif