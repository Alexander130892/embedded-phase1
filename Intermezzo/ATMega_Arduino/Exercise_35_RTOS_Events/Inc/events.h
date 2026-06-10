/*
 * File:    events.h
 * Author:  Alexander130892
 * Date:    10-6-2026
 *
 * Description:
 *   This header file defines an event system with a fixed-size queue
 *   for handling various appliance events (button presses, door
 *   status, water level, timeouts, motor errors) in an embedded
 *   system, providing functions to post, retrieve, and log events.
 */
#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>

#define EVENT_QUEUE_SIZE 8
#define TIMEOUT_BUZZER_START  1
#define TIMEOUT_BUZZER_STOP   2

typedef enum {
    EV_NONE,
    EV_BUTTON_START,
    EV_BUTTON_CANCEL,
    EV_DOOR_OPEN,
    EV_DOOR_CLOSED,
    EV_WATER_LEVEL_OK,
    EV_TIMEOUT,
    EV_MOTOR_ERROR,
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