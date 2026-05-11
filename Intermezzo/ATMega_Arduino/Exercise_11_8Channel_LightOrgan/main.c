
/*
    Notes:
        ADC needed for Potmeter speed regulation
            Bit out of scope for lesson, but implemented after some research
        Simple for loop used as delay (no Timers used)
        Setting patterns with bitshifts:
            Excercise mention to use this method, but not all desired patters can be calculated mathematicaly (e.g bounce)
            Thought it more oppurtune to have fixed look-up array and consistent method for most patterns (except bitcounter)

    Mapping:
        Led 0   -   D0  -   PD0     //using PD0/PD1 means UART TX/RX is not available
        Led 1   -   D1  -   PD1
        Led 2   -   D2  -   PD2
        Led 3   -   D3  -   PD3
        Led 4   -   D4  -   PD4
        Led 5   -   D5  -   PD5
        Led 6   -   D6  -   PD6
        Led 7   -   D7  -   PD7
        Potmeter    A5 -    PC5

    Wiring:
    PDx -- LEDs -- 220 OHm -- GND
    Potmeter(1) Vcc -- Potmeter(2) PB5 --  Potmeter (3) GND

*/
#include <avr/io.h>
#include <stdint.h>

/*
Exercise:
    Write a complete byte to a port to control 8 bytes at once
    Create at least 6 patters, save pattern in array of bytes
    Use potmeter to control speed
    Explain why this is faster than one-by-one control
 */

//   Register map (from ATmega328P datasheet §14):
//  *   DDRD/C  = Data Direction Register D/C  (1 = output, 0 = input)
//  *   PORTD = Port D Data Register       (write output value)
//  *   PINC = Port C Input Pins Register (read input value)

const uint8_t sweep[]       = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
const uint8_t bounce[]      = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,0x40, 0x20, 0x10, 0x08, 0x04, 0x02 };
const uint8_t fill[]        = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };
const uint8_t empty[]       = { 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
const uint8_t alternating[] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };
const uint8_t *patterns[]   = { sweep, bounce, fill, empty, alternating }; //array of pointers to patterns, since patterns have different lenghts (ragged array)
const uint8_t lengths[]     = { 8, 14, 8, 8, 8};

void adc_init(void);
uint16_t adc_read(uint8_t channel);

int main(void)
{
    DDRD    |= 0xFF;  //PD0-7 defined as output
    DDRC    &= ~(1u << 5);  //PC5 defined as input
    
    adc_init(); //need ADC to use potmeter as speed

    for(;;) { // infinite loop
        int binary_counter = 0; //select one if binary counter is desired
        int choice = 1 ; // Sweep = 0, bounce = 1, fill = 2, empty =3, alt=4
        
        uint16_t speed = adc_read(5);   // PC5 = channel 5, returns 0-1023
        uint32_t delay_count = 10000 + ((uint32_t)speed * speed / 10);
        if(!binary_counter)
        {
          for(uint8_t index = 0 ; index < lengths[choice] ; index++){
            PORTD = patterns[choice][index];
            speed = adc_read(5);                          // read every step
            delay_count = 10000 + ((uint32_t)speed * speed / 10);
            for(volatile uint32_t d = 0; d < delay_count; d++);
            }
        }
        else
        {
            // handle biinary counter, as it makes no sense to hardcode(256 values)
            for(uint8_t index = 0; index < 255; index++) {
                PORTD = patterns[choice][index];
                speed = adc_read(5);                          // read every step
                delay_count = 10000 + ((uint32_t)speed * speed / 10);
                for(volatile uint32_t d = 0; d < delay_count; d++);
            }
        }        
    }
    return 0;   /* never reached */
}

void adc_init(void) {
    ADMUX  = (1 << REFS0);          // AVcc as reference, channel 0 (default)
    ADCSRA = (1 << ADEN)  |         // enable ADC
             (1 << ADPS2) |         // prescaler 128 → 16MHz/128 = 125kHz
             (1 << ADPS1) |         //   (ADC needs 50-200kHz to work correctly)
             (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);  // select channel, keep reference bits
    ADCSRA |= (1 << ADSC);                       // start conversion
    while (ADCSRA & (1 << ADSC));               // wait until complete
    return ADC;                                  // read 10-bit result (0-1023)
}