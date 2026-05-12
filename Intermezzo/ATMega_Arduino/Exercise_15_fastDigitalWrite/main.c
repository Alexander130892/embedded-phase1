
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

/* 
asm dissection

fast write x: only 4 CPU cycles needed, but, very dedicated (not flexible)
compiler used sbi directly, single instruction for the set. The 4 cycles include the and (check bool), breq (branch), sbi (set bit), ret
00000080 <fastWrite7>:
  80:	88 23       	and	r24, r24
  82:	11 f0       	breq	.+4      	; 0x88 <.L2>
  84:	5f 9a       	sbi	0x0b, 7	; 11
  86:	08 95       	ret

6 CPU cycles needed (including error checking)
no sbi/cbi here because the pin number isn't known at compile time — it's a variable. 
sbi/cbi require a compile-time constant bit number, so the compiler falls back to a read-modify-write sequence: in (read PORTD), compute mask, XOR, out (write back).
--> flexibility costs instructions.

0000008c <fastToggle>:
  8c:	83 70       	andi	r24, 0x03	; 3
  8e:	9b b1       	in	r25, 0x0b	; 11
  90:	8c 5f       	subi	r24, 0xFC	; 252
  92:	21 e0       	ldi	r18, 0x01	; 1
  94:	30 e0       	ldi	r19, 0x00	; 0
  96:	01 c0       	rjmp	.+2      	; 0x9a <.L2^B2>


from Arduino IDE:
Lots more instructions
--> maximum flexibility, at cost of raw speed

void pinMode(uint8_t pin, uint8_t mode)
{
    uint8_t bit = digitalPinToBitMask(pin);
1e6:  e2 e9    ldi   r30, 0x92     ; load lookup table address
1e8:  f0 e0    ldi   r31, 0x00
1ea:  24 91    lpm   r18, Z        ; load bit mask from Flash

    uint8_t port = digitalPinToPort(pin);
1ec:  ee e7    ldi   r30, 0x7E
1ee:  f0 e0    ldi   r31, 0x00
1f0:  84 91    lpm   r24, Z        ; load port from Flash

    if (port == NOT_A_PIN) return;
1f2:  88 23    and   r24, r24
1f4:  99 f0    breq  .+38          ; bail if invalid pin

    reg = portModeRegister(port);  ; calculate DDR address
1f6:  90 e0    ldi   r25, 0x00
1f8:  88 0f    add   r24, r24
1fa:  99 1f    adc   r25, r25
...
        uint8_t oldSREG = SREG;
210:  8f b7    in    r24, 0x3f     ; save interrupt state
        cli();
212:  f8 94    cli                 ; disable interrupts
        *reg |= bit;
214:  ec 91    ld    r30, X        ; read DDR
216:  e2 2b    or    r30, r18      ; set bit
218:  ec 93    st    X, r30        ; write DDR
        SREG = oldSREG;
21a:  8f bf    out   0x3f, r24     ; restore interrupts

*/