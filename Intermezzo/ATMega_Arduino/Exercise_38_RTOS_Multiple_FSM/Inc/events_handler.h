/*
 * File:    events_handler.h
 * Author:  Alexander130892
 * Date:    13-6-2026
 *
 * Description:
 *   This header file declares functions for handling and processing
 *   events, serving as the interface for an event management system
 *   that works with Event structures defined in "events.h".
 */

// event_handler.h
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "events.h"

void handleEvent(Event ev);
void processEvents(void);

#endif