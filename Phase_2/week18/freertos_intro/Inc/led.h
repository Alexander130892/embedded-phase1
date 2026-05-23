

#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include <stdint.h>
#include "status.h"

status_t LED_init(void);
status_t LED_toggle(void);

#endif
