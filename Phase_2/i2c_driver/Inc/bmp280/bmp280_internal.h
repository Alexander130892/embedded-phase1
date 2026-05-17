/*
 * File:    bmp280_internal.h
 * Author:  Alexander130892
 * Date:    16-5-2026
 *
 * Description:
 *   This internal header defines register addresses, calibration
 *   parameter types, and compensation function prototypes for the
 *   BMP280 pressure/temperature sensor driver. It is intended for use
 *   only within the driver's own translation units, declaring the
 *   shared `comp_param_t` structure that holds factory calibration
 *   coefficients used by the temperature and pressure compensation
 *   calculations.
 */
#ifndef BMP280_BMP280_INTERNAL_H_
#define BMP280_BMP280_INTERNAL_H_

//REGISTER ADDRESSES
#define WHOAMI		0xD0
#define	TEMP_XLSB	0xFC
#define	TEMP_LSB	0xFB
#define	TEMP_MSB	0xFA

#define	PRESS_XLSB	0xF9
#define	PRESS_LSB	0xF8
#define	PRESS_MSB	0xF7

#define	CONFIG		0xF5
#define	CTRL_MEAS	0xF4
#define	STATUS		0xF3
#define	RESET		0xE0

#define CALIB_START	0x88 //24bytes until CALIB_END
#define CALIB_END	0xA1


typedef struct {
	uint16_t 	dig_T1;
	int16_t 	dig_T2;
	int16_t 	dig_T3;
	uint16_t 	dig_P1;
	int16_t 	dig_P2;
	int16_t 	dig_P3;
	int16_t 	dig_P4;
	int16_t 	dig_P5;
	int16_t 	dig_P6;
	int16_t 	dig_P7;
	int16_t 	dig_P8;
	int16_t 	dig_P9;
} __attribute__((packed)) comp_param_t; //Compensation parameters
// Called by init(), static between calls, read by every T/P-read
extern comp_param_t comp_params; //Compensation parameters

int32_t bmp280_compensate_temp(uint32_t adc_T);
int32_t bmp280_compensate_pressure(uint32_t adc_P, uint32_t adc_T);

#endif /* BMP280_BMP280_INTERNAL_H_ */
