/*
 * File:    events_handler.h
 * Author:  Alexander130892
 * Date:    10-6-2026
 *
 * Description:
 *   This header file declares a function `handleEvent()` that
 *   processes events of type `Event`, serving as the interface for
 *   event dispatching in the application. It includes the events
 *   definition header to access the Event type.
 */

// event_handler.h
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "events.h"

void handleEvent(Event ev);

#endif