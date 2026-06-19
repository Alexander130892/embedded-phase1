/*
 * File:    task.h
 * Author:  Alexander130892
 * Date:    16-6-2026
 *
 * Description:
 *  
 */
#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"

typedef void (*task_function_t)(void);
typedef struct {
    task_function_t function;
    uint32_t period;
    uint32_t last_run;
} task_t;

extern volatile uint32_t idle_count;

void schedulerRun(void);

#endif