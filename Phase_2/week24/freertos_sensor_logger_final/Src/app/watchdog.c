/*
 * File:    watchdog.c
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This watchdog file monitors the responsiveness of two sensors
 *   (MPU6050 and BMP280) by periodically checking if they've cleared
 *   their timeout flags, and notifies a logging task if either sensor
 *   fails to respond within the timer interval.
 */
#include "app/watchdog.h"

volatile bool b_timeout_mpu6050 = false;
volatile bool b_timeout_bmp280 = false;

void vHeartBeatTimer( TimerHandle_t xTimerHeartBeat ){
	if(b_timeout_mpu6050){
		xTaskNotifyGive(xLogTaskHandle);			//notify vUARTTask
	}else{
		b_timeout_mpu6050 = true; 		//reset -- sensor should set before next timer-callback
	}
	if(b_timeout_bmp280){
		xTaskNotifyGive(xLogTaskHandle);			//notify vUARTTask
	}else{
		b_timeout_bmp280 = true; 		//reset -- sensor should set before next timer-callback
	}
}
