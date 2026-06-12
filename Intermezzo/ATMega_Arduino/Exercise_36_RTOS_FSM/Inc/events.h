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
#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>

#define EVENT_QUEUE_SIZE 8
#define TIMEOUT_BUZZER_START  1
#define TIMEOUT_BUZZER_STOP   2
#define TIMEOUT_DOOR_AUTO_CLOSE   3

typedef enum {
    EV_NONE,
    EV_BUTTON_START,
    EV_BUTTON_CANCEL,
    EV_DOOR_OPEN,
    EV_DOOR_CLOSED,
    EV_WATER_LEVEL_OK,
    EV_TIMEOUT,
    EV_MOTOR_ERROR,
    EV_LIMIT_OPEN,
    EV_LIMIT_CLOSED,
    EV_COUNT
} EventType;

typedef struct {
    EventType type;
    int value;
} Event;


extern Event queue[EVENT_QUEUE_SIZE];

bool postEvent(Event event);
bool getEvent(Event *event);
void logEvent(Event event);

#endif