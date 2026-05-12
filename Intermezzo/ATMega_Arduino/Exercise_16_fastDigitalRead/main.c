
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

*/
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/*
Exercise:
    Couple arduino-pins to AVR-ports and bits
    make a function fastRead()
    Create a scanner which keeps track of which button has been pressed
    compare current state vs previous state
    show current status w/ LEDs
    Design a small HAL
 */

//   Register map (from ATmega328P datasheet §14):
//  *   DDRD  = Data Direction Register D  (1 = output, 0 = input)
//  *   PORTD = Port D Data Register       (write output value)
//  *   PIND = Port D Input Pins Register (read input value)

uint8_t fastDigitalReadD(uint8_t pin); //Read port D completely
void fastDigitalWrite(uint8_t pin, bool value);
uint8_t portScanD();
void portLED(uint8_t state);

int main(void)
{
    DDRD = 0xF0;    // PD0-3 Input, PD4-7 output;
    PORTD = 0x0F;   // PD0-3 pull-up resistor activated - Active low
    uint8_t prev_state=0x0F, 
            current_state=0x0F,
            pressed_now = 0x00; // init --> No LEDS
    for(;;) { // infinite loop
        current_state = portScanD();
        pressed_now = current_state & ~ prev_state;
        prev_state = current_state;
        portLED(pressed_now);
        for(volatile uint32_t d = 0; d < 100000 ; d++ ); // short delay       
        
    }       
    return 0;   /* never reached */
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
uint8_t fastDigitalReadD(uint8_t pin){
    return (PIND & (1u << pin));
}
uint8_t portScanD(){
    uint8_t state = PIND & 0xF;
    return (state);
}
void portLED(uint8_t state){
    PORTD = (state << 4) | 0x0F;
}