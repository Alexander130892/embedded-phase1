/*
 * File:    bmp280_internal.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This header file defines the internal register addresses,
 *   compensation parameters, and function declarations for a BMP280
 *   barometric pressure and temperature sensor driver. It provides the
 *   necessary constants for SPI communication, calibration data
 *   structures, and compensation algorithms to read and correct raw
 *   sensor measurements.
 */
#ifndef BMP280_BMP280_INTERNAL_H_
#define BMP280_BMP280_INTERNAL_H_

//REGISTER ADDRESSES
#define BMP280_WHOAMI		0xD0
#define	BMP280_TEMP_XLSB	0xFC
#define	BMP280_TEMP_LSB		0xFB
#define	BMP280_TEMP_MSB		0xFA

#define	BMP280_PRESS_XLSB	0xF9
#define	BMP280_PRESS_LSB	0xF8
#define	BMP280_PRESS_MSB	0xF7

#define	BMP280_CONFIG		0xF5
#define	BMP280_CTRL_MEAS	0xF4
#define BMP280_CTRL_MEAS_VALUE 0x93	// Oversampling x 8 for P and T, Normal Power mode
#define	BMP280_STATUS		0xF3
#define	BMP280_RESET		0xE0

#define BMP280_CALIB_START	0x88 //24bytes until CALIB_END
#define BMP280_CALIB_END	0xA1

#define BMP280_SPI_READ   	0x80
#define BMP280_SPI_WRITE  	0x7F  // mask for clearing bit 7


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

extern comp_param_t comp_params; //Compensation parameters

int32_t bmp280_compensate_temp(uint32_t adc_T);
int32_t bmp280_compensate_pressure(uint32_t adc_P, uint32_t adc_T);


#endif /* BMP280_BMP280_INTERNAL_H_ */
