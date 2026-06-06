/*
 * watchdog.c
 *
 *  Created on: Jun 6, 2026
 *      Author: alexandervindelinckx
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
