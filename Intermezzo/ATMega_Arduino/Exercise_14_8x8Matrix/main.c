
/*
    Notes:
        MAX7219 driver chip handles multiplexing internally --> no need for refresh matrix
        Need SPI to transfer data
            --> probably not the aim of the excercise, but tried it for other learning purpose
            also, I don't have 64 LEDs
            --> used it as an excercise on SPI
    Mapping:
        VCC     -   5V
        GND     -   GND
        CS      -   Pin 10 (SS)
        DIN     -   Pin 11 (MOSI)
        CLK     -   Pin 13 (SCK)


    MAX7219 protocol:
        Every transaction is 16 bits: [8-bit register address | 8-bit data], framed by CS low/high:
        CS low > send address byte > send data byte > CS high

    Register map (MAX7219 datasheet):
        0x00    No-op
        0x01    Row 1
        ...
        0x08    Row 8
        0x09    Decode mode
        0x0A    Intensity
        0x0B    Scan limit
        0x0C    Shutdown
        0x0F    Display test
    Link Datasheet:
        https://cdn.shopify.com/s/files/1/1509/1638/files/AZ039_A_7-13_EN_B07CRF13ZQ_214c9067-2dde-4ce9-9f90-943eb3046e57.pdf?v=1721043187
*/
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

//SPI
#define CS_DDR      DDRB
#define CS_PORT     PORTB
#define CS_PIN      PB2     // D10

//MAX7219 register addresses
#define REG_NOOP        0x00
#define REG_DIGIT0      0x01    // Row 1 (top)
#define REG_DECODE      0x09
#define REG_INTENSITY   0x0A
#define REG_SCANLIMIT   0x0B
#define REG_SHUTDOWN    0x0C
#define REG_DISPTEST    0x0F

/*
Exercise:
    8x8 Matrix - MAX7219 driver chip
        Define bitmap for one symbol
        write refresh function, which acts row by row
        Show at least 3 symbols
        Animate (/scroll)
 */
uint8_t smiley[8] = {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C};
uint8_t heart[8]  = {0x00, 0x66, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00};
uint8_t cross[8]  = {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81};
uint8_t arrow[8]  = {0x18, 0x3C, 0x7E, 0xFF, 0x18, 0x18, 0x18, 0x18};

void spi_init(void);
void spi_send(uint8_t data);
void max7219_write(uint8_t reg, uint8_t data);
void max7219_init(void);
void show(const uint8_t image[8]);
void scroll(const uint8_t from[8], const uint8_t to[8], uint16_t delay_ms);

int main(void)
{
    spi_init();
    max7219_init();
 
    for(;;) {
        // Show three static symbols
        // show(smiley);
        // for(volatile uint32_t d = 0; d < 100000; d++);
 
        show(heart);
        for(volatile uint32_t d = 0; d < 1000000; d++);
 
    //     show(cross);
    //    for(volatile uint32_t d = 0; d < 1000000; d++);
 
    //     // Scroll smiley → heart → arrow
    //     scroll(smiley, heart,  50);
    //     scroll(heart,  arrow,  50);
    //     scroll(arrow,  smiley, 50);
    }
 
    return 0;
}
// SPI
void spi_init(void)
{
    // CS, MOSI, SCK as output
    CS_DDR  |=  (1 << CS_PIN);
    DDRB    |=  (1 << PB3) | (1 << PB5);
    CS_PORT |=  (1 << CS_PIN);      // CS idle high
 
    SPCR = (1 << SPE)  |    //  SPI enable
           (1 << MSTR) |    //  Master mode
           (1 << SPR0);     //  clock = fosc/16 (1 MHz @ 16 MHz)
}
// Send one byte over SPI
void spi_send(uint8_t data)
{
    SPDR = data;                    // load shift register > starts transmission
    while (!(SPSR & (1 << SPIF))); // wait for transfer complete flag
}
// Send 16-bit command to MAX7219
void max7219_write(uint8_t reg, uint8_t data)
{
    CS_PORT &= ~(1 << CS_PIN);  // CS low  > latch open
    spi_send(reg);
    spi_send(data);
    CS_PORT |=  (1 << CS_PIN);  // CS high > latch closes, MAX7219 accepts data
}
// Wake up and configure MAX7219
void max7219_init(void)
{
    max7219_write(REG_DISPTEST,  0x00); // display test off
    max7219_write(REG_SHUTDOWN,  0x01); // normal operation (exit shutdown)
    max7219_write(REG_DECODE,    0x00); // no BCD decode — raw bitmap mode
    max7219_write(REG_SCANLIMIT, 0x07); // scan all 8 rows
    max7219_write(REG_INTENSITY, 0x08); // brightness 0x00 (min) – 0x0F (max)
}
// Write 8-row bitmap to display
void show(const uint8_t image[8])
{
    for(uint8_t row = 0; row < 8; row++) {
        max7219_write(REG_DIGIT0 + row, image[row]);
    }
}
// Scroll from one bitmap to another (left scroll)
void scroll(const uint8_t from[8], const uint8_t to[8], uint16_t delay_ms)
{
    // build 16-column buffer: [from | to]
    uint8_t buf[16];
    for(uint8_t i = 0; i < 8; i++) {
        buf[i]     = from[i];
        buf[i + 8] = to[i];
    }
    // shift left one column at a time, 8 steps
    for(uint8_t shift = 0; shift < 8; shift++) {
        for(uint8_t row = 0; row < 8; row++) {
            // take 8 bits starting at 'shift' from the 16-bit row
            uint8_t combined = (buf[row] << shift) | (buf[row + 8] >> (8 - shift));
            max7219_write(REG_DIGIT0 + row, combined);
        }
        for(volatile uint32_t d = 0; d < delay_ms*1000; d++);
    }
}