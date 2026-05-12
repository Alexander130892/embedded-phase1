
/*
    Notes:
        For practical reasons, only 4 LEDS are used iso 6
        see (dis)asm comparison below the code for comparison
        
    Mapping:
        Button 0   -   D0  -   PD0     //using PD0/PD1 means UART TX/RX is not available
        Button 1   -   D1  -   PD1
        Button 2   -   D2  -   PD2
        Button 3   -   D3  -   PD3
        Led 4      -   D4  -   PD4
        Led 5      -   D5  -   PD5
        Led 6      -   D6  -   PD6
        Led 7      -   D7  -   PD7

    Wiring:
    PDx -- LEDs -- 220 OHm -- GND
    Potmeter(1) Vcc -- Potmeter(2) PB5 --  Potmeter (3) GND

*/
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/*
Exercise:
    Couple arduino-pins to AVR-ports and bits
    Visualise library function vs custom function
    Use bitmask to act on one specific bit
    Design a small HAL
 */

//   Register map (from ATmega328P datasheet §14):
//  *   DDRD  = Data Direction Register D  (1 = output, 0 = input)
//  *   PORTD = Port D Data Register       (write output value)
//  *   PINC = Port C Input Pins Register (read input value)

void fastWrite7(bool value);
void fastDigitalWrite(uint8_t pin, bool value);
void fastToggle(uint8_t pin);

int main(void)
{
    DDRD = 0xF0;    // PD0-3 Input, PD4-7 output;
    PORTD = 0x0F;   // PD0-3 pull-up resistor activated - Active low
    
    for(;;) { // infinite loop
        // blinks LED 7
        fastWrite7(true);
        for(volatile uint32_t d = 0; d < 100000 ; d++ ); // simple delay
        fastWrite7(false);
        for(volatile uint32_t d = 0; d < 100000 ; d++ ); // simple delay
        
        // blinks LED6
        fastDigitalWrite(6, true);
        for(volatile uint32_t d = 0; d < 100000 ; d++ ); // simple delay
        fastDigitalWrite(6, false);
        for(volatile uint32_t d = 0; d < 100000 ; d++ ); // simple delay

        // blinks LED5
        fastToggle(5);
        for(volatile uint32_t d = 0; d < 100000 ; d++ ); // simple delay
        fastToggle(5);
        for(volatile uint32_t d = 0; d < 100000 ; d++ ); // simple delays
    }       
    return 0;   /* never reached */
}

void fastWrite7(bool value){
    if(value)
    {
        PORTD |= (0x80); // 1000 0000 > Set PD7 > D7
    }else
    {
        PORTD &= ~(0x80);
    }
}
void fastDigitalWrite(uint8_t pin, bool value){
    pin = (pin % 4) + 4;            // map bits < 4 to 4-7            
    uint8_t mask = (1u << pin);
    if(value)
    {
        PORTD |= (mask); 
    }else
    {
        PORTD &= ~(mask);
    }
}

void fastToggle(uint8_t pin){
    pin = (pin % 4) + 4;            // map bits < 4 to 4-7            
    uint8_t mask = (1u << pin);
    PORTD ^= (mask);
}