// event_handler.h
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "events.h"

void handleEvent(event_t ev);
void processEvents(void);
void logEvent(event_t event);


#endif