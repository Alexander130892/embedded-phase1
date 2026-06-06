/*
 * watchdog.h
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
 */

#ifndef APP_WATCHDOG_H_
#define APP_WATCHDOG_H_

#include <stdbool.h>
#include "rtos/kernel_objects.h"

#define HEARTBEAT_PERIOD	5000

void vHeartBeatTimer 	(TimerHandle_t xTimerHeartBeat );

extern volatile bool b_timeout_mpu6050;
extern volatile bool b_timeout_bmp280;

#endif /* APP_WATCHDOG_H_ */
