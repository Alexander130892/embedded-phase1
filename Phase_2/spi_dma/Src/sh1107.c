/*
 * File:    sh1107.c
 * Author:  Alexander130892
 * Date:    11-5-2026
 *
 * Description:
 *   This file implements a device driver for the SH1107 OLED display
 *   controller, providing functions to initialize the display, send
 *   commands and data via SPI communication, clear the screen, and
 *   draw characters using an 8x8 font. The driver uses direct register
 *   manipulation for GPIO control and includes both standard and
 *   DMA-based data transfer methods for updating the 128x128 pixel
 *   display.
 */
#include "sh1107.h"
#include "font8x8.h"

void sh1107_init(){
    // Hardware reset
    *((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) &= ~(1u << 3);
    for(volatile uint32_t d = 0; d < 800000; d++);
    *((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) |=  (1u << 3);

    sh1107_send_cmd(0xAE);            // Display OFF
    sh1107_send_cmd_data(0xD5, 0x10); // Clock divide
    sh1107_send_cmd_data(0xA8, 0x7F); // Multiplex ratio 128
    sh1107_send_cmd_data(0xD3, 0x00); // Display offset = 0 for 128x128
    sh1107_send_cmd_data(0xDC, 0x00); // Start line = 0
    sh1107_send_cmd(0xA0);            // Segment remap
    sh1107_send_cmd(0xC8);            // COM scan direction
    sh1107_send_cmd_data(0x81, 0xFF); // Contrast
    sh1107_send_cmd_data(0xAD, 0x8B); // DC-DC on
    sh1107_send_cmd_data(0xD9, 0xF1); // Pre-charge
    sh1107_send_cmd_data(0xDB, 0x35); // VCOM deselect
    sh1107_send_cmd(0xA4);            // Resume RAM
    sh1107_send_cmd(0xA6);            // Normal display
    // 100ms delay before display on
    for(volatile uint32_t d = 0; d < 800000; d++);
    sh1107_send_cmd(0xAF);            // Display ON
}

void sh1107_send_cmd(uint8_t cmd){
	//CS low
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET))	&= 		~(1u << 4); 		// PB4
	//DC low (command mode)
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET))	&= 		~(1u << 5); 		// PB5
	//Send command byte
	spi_transfer(cmd);
	//Wait until BSY status cleared before reseting CS to not clip
	while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) );
	//CS high
	*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET))	|= 		(1u << 4); 		// PB4
}

void sh1107_send_cmd_data(uint8_t cmd, uint8_t data){
	//CS low
		*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET))	&= 		~(1u << 4); 		// PB4
		//DC low (command mode)
		*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET))	&= 		~(1u << 5); 		// PB5
		//Send command byte
		spi_transfer(cmd);
		//Wait until BSY status cleared before sending data
		while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) );
		//DC High (Data mode)
		*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET))	|= 		(1u << 5); 		// PB5
		//Send command byte
		spi_transfer(data);
		//Wait until BSY status cleared before reseting CS to not clip
		while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) );
		//CS high
		*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET))	|= 		(1u << 4); 		// PB4
}
void sh1107_send_data(uint8_t data){
    // CS low
    *((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) &= ~(1u << 4);
    // DC high (data mode)
    *((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) |=  (1u << 5);
    // Send data byte
    spi_transfer(data);
    // Wait for BSY to clear
    while( *((volatile uint32_t *)(SPI1_BASE_ADDR + SPIx_SR_OFFSET)) & (1u << 7) );
    // CS high
    *((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) |=  (1u << 4);
}

void sh1107_clear(){
    for (uint8_t page = 0; page < 16; page++) {
        sh1107_send_cmd(0xB0 | page);   // Set page address
        sh1107_send_cmd(0x00);          // Set lower column = 0
        sh1107_send_cmd(0x10);          // Set higher column = 0
        for (uint8_t col = 0; col < 128; col++) {
            sh1107_send_data(0x00);
        }
    }
}

void sh1107_oled_dma_write(uint8_t* data){
	for (uint8_t page = 0; page < 16; page++) {
	        sh1107_send_cmd(0xB0 | page);   // Set page address
	        sh1107_send_cmd(0x00);          // Set lower column = 0
	        sh1107_send_cmd(0x10);          // Set higher column = 0
	        // DC high (data mode)
			*((volatile uint32_t *)(GPIOB_BASE_ADDR + GPIOx_ODR_OFFSET)) |=  (1u << 5);
			spi_dma_write(data+page*128, 128);
			while(dma_s3_flag == 0){};
			dma_s3_flag = 0;
	}
}

void sh1107_draw_char(uint8_t* fb, uint8_t char_col, uint8_t char_row, char c){
    if (c < 32 || c > 127) c = 32;
    const uint8_t* glyph = font8x8[c - 32];
    for (uint8_t i = 0; i < 8; i++){
        uint8_t byte_val = glyph[i];
        for (uint8_t b = 0; b < 8; b++){
        	if (byte_val & (1 << (b))){
                fb[char_col * 128 + char_row * 8 + b] |= (1 << i);
            }
        }
    }
}

void sh1107_draw_string(uint8_t* fb, uint8_t char_col, uint8_t char_row, const char* str){
    while (*str){
        sh1107_draw_char(fb, char_col, char_row, *str);
        char_col++;
        if (char_col >= 16) break;
        str++;
    }
}

