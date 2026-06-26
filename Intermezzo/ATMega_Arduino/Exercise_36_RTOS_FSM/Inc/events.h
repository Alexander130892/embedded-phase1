/*
 * File:    events.h
 * Author:  Alexander130892
 * Date:    26-6-2026
 *
 * Description:
 *   This header file defines an event queue system for a washing
 *   machine controller, declaring event types (button presses,
 *   door/water/motor states), a fixed-size queue, and functions to
 *   post, retrieve, and log events.
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
} event_type_t;

typedef struct {
    event_type_t type;
    int value;
} event_t;

extern event_t queue[EVENT_QUEUE_SIZE];

bool postEvent(event_t event);
bool getEvent(event_t *event);
void logEvent(event_t event);

#endif