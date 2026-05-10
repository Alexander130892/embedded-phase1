/*
 * sh1107.h
 *
 *  Created on: May 9, 2026
 *      Author: alexandervindelinckx
 */

#ifndef SH1107_H_
#define SH1107_H_

/*
 Pin mapping is:

GND — Ground
VCC — 3.3V
SCL — SPI Clock 							(PA5 - shared)
SDA — MOSI, data in 						(PA7 - shared )
RES — Hardware reset 						(PB3)
DC — Data/Command select 					(PB5)
CS — Chip Select 							(PB4)

Note there is no MISO — the SH1107 is write-only over SPI.
 */

#include <stdint.h>
#include "stm32f446re.h"
#include "spi.h"


void sh1107_init();
void sh1107_send_cmd(uint8_t cmd);
void sh1107_send_cmd_data(uint8_t cmd, uint8_t data);
void sh1107_send_data(uint8_t data);
void sh1107_clear();
void sh1107_oled_dma_write();
void sh1107_draw_char(uint8_t* fb, uint8_t x, uint8_t page, char c);
void sh1107_draw_string(uint8_t* fb, uint8_t x, uint8_t page, const char* str);


#endif /* SH1107_H_ */
