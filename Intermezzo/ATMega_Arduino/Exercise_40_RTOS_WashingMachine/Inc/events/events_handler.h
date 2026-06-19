/*
 * File:    events_handler.h
 * Author:  Alexander130892
 * Date:    19-6-2026
 *
 * Description:
 *   This header file declares the interface for an event handling
 *   system that processes events, executes corresponding handlers, and
 *   logs event occurrences. It provides three main functions to manage
 *   event lifecycle: handling individual events, processing queued
 *   events, and recording events for auditing purposes.
 */

// event_handler.h
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "events.h"

void handleEvent(event_t ev);
void processEvents(void);
void logEvent(event_t event);


#endif