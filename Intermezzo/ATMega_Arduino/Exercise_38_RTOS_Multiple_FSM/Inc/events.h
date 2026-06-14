/*
 * File:    events.h
 * Author:  Alexander130892
 * Date:    14-6-2026
 *
 * Description:
 *   This header file defines an event system for a washing machine
 *   controller, providing an event queue structure with various event
 *   types (button presses, door/water sensors, timeouts, errors) and
 *   functions to post, retrieve, and log events.
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
    EV_START,
    EV_PHASE_TIMEOUT,
    EV_WATER_TIMEOUT,
    EV_CANCEL,
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