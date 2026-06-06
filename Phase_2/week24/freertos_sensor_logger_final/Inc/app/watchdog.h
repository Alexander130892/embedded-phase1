/*
 * File:    watchdog.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file defines a watchdog timer interface that monitors
 *   sensor health by tracking timeouts for MPU6050 and BMP280 sensors,
 *   with a heartbeat timer callback function that executes every 5
 *   seconds.
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
