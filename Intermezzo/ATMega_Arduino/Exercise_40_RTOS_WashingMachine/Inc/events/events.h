/*
 * File:    events.h
 * Author:  Alexander130892
 * Date:    15-6-2026
 *
 * Description:

 */
#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#include <stdbool.h>

#define EVENT_QUEUE_SIZE 8

typedef enum {
    EV_NONE,
    EV_DOOR_OPEN,
    EV_DOOR_CLOSED,
    EV_WATER_LEVEL_OK,
    EV_TIMEOUT,
    EV_MOTOR_ERROR,
    EV_START,
    EV_PHASE_TIMEOUT,
    EV_WATER_TIMEOUT,
    EV_CANCEL,
    EV_COUNT
} event_type_t;

typedef struct {
    event_type_t type;
    int8_t value;
} event_t;

bool postEvent(event_t event);
bool getEvent(event_t *event);

#endif