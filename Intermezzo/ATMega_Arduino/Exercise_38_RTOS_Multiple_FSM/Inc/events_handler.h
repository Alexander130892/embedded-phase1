/*
 * File:    events_handler.h
 * Author:  Alexander130892
 * Date:    14-6-2026
 *
 * Description:
 *   This header file declares functions to handle and process events,
 *   providing an interface for event-driven programming by defining
 *   callbacks for individual event handling and batch event
 *   processing.
 */
// event_handler.h
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "events.h"

void handleEvent(Event ev);
void processEvents(void);

#endif