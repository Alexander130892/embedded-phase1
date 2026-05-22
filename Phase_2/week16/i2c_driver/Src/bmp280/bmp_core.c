/*
 * File:    bmp_core.c
 * Author:  Alexander130892
 * Date:    22-5-2026
 *
 * Description:
 *   This file implements the BMP280 sensor's temperature and pressure
 *   compensation algorithms, applying the manufacturer-specified
 *   fixed-point formulas using factory-calibrated trimming parameters
 *   to convert raw ADC readings into meaningful physical values. It
 *   maintains a static `t_fine` intermediate temperature value shared
 *   between the two compensation functions, ensuring pressure readings
 *   are always temperature-corrected.
 */
#include "stm32f446re.h"
#include "bmp280.h"
#include "bmp280_internal.h"

static uint32_t t_fine;
comp_param_t comp_params;

// Called by init(), static between calls, read by every T/P-read

int32_t bmp280_compensate_temp(uint32_t adc_T){
	//Compensation formula
	int32_t var1, var2, T;
	var1 = ((((adc_T >> 3) - ((int32_t)comp_params.dig_T1 << 1))) * ((int32_t)comp_params.dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t)comp_params.dig_T1)) * ((adc_T >> 4) - ((int32_t)comp_params.dig_T1))) >> 12) *	((int32_t)comp_params.dig_T3)) >> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}
int32_t bmp280_compensate_pressure(uint32_t adc_P, uint32_t adc_T){
	bmp280_compensate_temp(adc_T);
	//Compensation formula
	int32_t var1, var2;
	uint32_t p;
	var1 = (((int32_t)t_fine)>>1) - 64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)comp_params.dig_P6);
	var2 = var2 + ((var1*((int32_t)comp_params.dig_P5))<<1);
	var2 = (var2>>2)+(((int32_t)comp_params.dig_P4)<<16);
	var1 = (((comp_params.dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)comp_params.dig_P2) * var1)>>1))>>18;
	var1 =((((32768+var1))*((int32_t)comp_params.dig_P1))>>15);
	if (var1 == 0)
	{
	return 0; // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
	if (p < 0x80000000)
	{
	p = (p << 1) / ((uint32_t)var1);
	}
	else
	{
	p = (p / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)comp_params.dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)comp_params.dig_P8))>>13;
	p = (uint32_t)((int32_t)p + ((var1 + var2 + comp_params.dig_P7) >> 4));
	return p;
}
