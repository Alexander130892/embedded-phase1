/*
 * File:    task.h
 * Author:  Alexander130892
 * Date:    10-6-2026
 *
 * Description:
 *   This header file defines a simple task scheduler interface with a
 *   Task structure that stores a function pointer, execution period,
 *   and last run time, along with a scheduler function to execute
 *   registered tasks.
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